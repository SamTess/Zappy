/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command
*/
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/server.h"
#include "../include/pending_cmd.h"
#include "../include/circular_buffer.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int check_disconnect(int bytes_read, client_t *user, server_t *server)
{
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            cleanup_client(user);
            remove_fd(server, user->client_poll->fd);
            return 1;
        }
        return 1;
    }
    return 0;
}

command_data_t get_command_data(void)
{
    static const char *comm_char[] = {"Forward", "Right", "Left",
        "Inventory", "Look", "Eject", "Connect_nbr", "Take", "Set",
        "Incantation", "Fork", "Broadcast", "msz", "bct", "mtc",
        "tna", "ppo", "plv", "pin", "sgt", "sst", NULL};
    static void (*comm_func[])(server_t *, client_t *, char *) =
        {forward, right, left, inventory, look, eject,
        connect_nbr, take_object, set_object, start_incantation,
        fork_c, broadcast, command_msz, command_bct, command_mtc,
        command_tna, command_ppo, command_plv, command_pin,
        command_sgt, command_sst, NULL};
    static int comm_times[] = {7, 7, 7, 1, 7, 7, 0, 7, 7, 300, 42, 7, 0,
        0, 0, 0, 0, 0, 0, 0, 0};
    static enum client_type_e accepted_types[] = {AI, AI, AI, AI, AI,
        AI, AI, AI, AI, AI, AI, AI, GRAPHICAL, GRAPHICAL, GRAPHICAL,
        GRAPHICAL, GRAPHICAL, GRAPHICAL, GRAPHICAL, GRAPHICAL};
    command_data_t data = {comm_char, comm_func, comm_times, accepted_types};

    return data;
}

static bool execute_graphical_command(server_t *server, client_t *user,
    char *buffer, int cmd_index)
{
    command_data_t data = get_command_data();

    data.functions[cmd_index](server, user, buffer);
    return true;
}

static bool execute_if_free(server_t *server, client_t *user,
    char *buffer, int cmd_index)
{
    if (user->type == GRAPHICAL)
        return execute_graphical_command(server, user, buffer, cmd_index);
    if (user->type == AI && user->player->busy_until <= server->current_tick) {
        add_pending_cmd(user, server, buffer, cmd_index);
        return true;
    } else {
        if (user->player->queue_size < 10) {
            add_to_command_queue(user, buffer);
            return true;
        } else
            return true;
    }
}

static bool find_and_execute(server_t *server, client_t *user, char *buffer)
{
    command_data_t data = get_command_data();

    for (int i = 0; data.commands[i] != NULL; i++) {
        if (strncmp(buffer, data.commands[i], strlen(data.commands[i])) == 0 &&
            user->type == data.accepted_types[i])
            return execute_if_free(server, user, buffer, i);
    }
    return false;
}

static bool is_valid_team_name(char *team_name, server_t *server,
    client_t *user)
{
    if (!team_name || !server ||
        !server->parsed_info || !server->parsed_info->names)
        return false;
    if (strlen(team_name) < 2 || team_name[strlen(team_name) - 1] != '\n')
        return false;
    team_name[strlen(team_name) - 1] = '\0';
    if (strcmp(team_name, "GRAPHIC") == 0) {
        user->type = GRAPHICAL;
        user->player = NULL;
        return true;
    }
    for (int i = 0; server->parsed_info->names[i] != NULL; i++) {
        if (strcmp(team_name, server->parsed_info->names[i]) == 0){
            user->player->team_name = strdup(team_name);
            user->type = AI;
            init_new_player_pos(server, user);
            return true;
        }
    }
    return false;
}

static void send_info_new_client(server_t *server, client_t *user)
{
    char *tmp_string = NULL;
    int len1 = snprintf(NULL, 0, "%d\n", user->client_id);
    int len2 = snprintf(NULL, 0, "%d %d\n",
        server->parsed_info->width,
        server->parsed_info->height);

    tmp_string = malloc(len1 + 1);
    sprintf(tmp_string, "%d\n", connect_nbr_srv(server,
        user->player->team_name));
    write_command_output(user->client_fd, tmp_string);
    free(tmp_string);
    tmp_string = malloc(len2 + 1);
    sprintf(tmp_string, "%d %d\n",
        server->parsed_info->width,
        server->parsed_info->height);
    write_command_output(user->client_fd, tmp_string);
    free(tmp_string);
    send_pnw_command_to_all(server, user);
}

void execute_com(server_t *server, client_t *user, char *buffer)
{
    if (!user)
        return;
    if (!user->is_fully_connected) {
        if (!is_valid_team_name(buffer, server, user)
            || (user->type != GRAPHICAL &&
                connect_nbr_srv(server, user->player->team_name) < 0))
            return write_command_output(user->client_fd, "ko\n");
        user->is_fully_connected = true;
        if (user->type == GRAPHICAL) {
            add_graphic_client(server, user);
            send_map_info_to_one_client(server, user);
            return;
        } else
            return send_info_new_client(server, user);
    }
    if (!find_and_execute(server, user, buffer))
        write_command_output(user->client_fd, "ko\n");
}

static void check_command(circular_buffer_t *temp_buffer, int cmd_length,
    server_t *server, client_t *user)
{
    char *command;

    command = extract_command(temp_buffer, cmd_length);
    if (command) {
        execute_com(server, user, command);
        free(command);
    }
}

void get_message(server_t *server, client_t *user)
{
    circular_buffer_t temp_buffer;
    char byte;
    int bytes_read;
    int cmd_length;

    init_circular_buffer(&temp_buffer);
    while (1) {
        bytes_read = read(user->client_poll->fd, &byte, 1);
        if (check_disconnect(bytes_read, user, server) == 1)
            return;
        if (add_to_circular_buffer(&temp_buffer, byte) == -1) {
            write_command_output(user->client_fd, "ko\n");
            return;
        }
        cmd_length = find_command_end(&temp_buffer);
        if (cmd_length > 0) {
            check_command(&temp_buffer, cmd_length, server, user);
            break;
        }
    }
}
