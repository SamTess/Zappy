/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command
*/
#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/pending_cmd.h"
#include "../include/circular_buffer.h"
#include "../include/parsing.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

command_data_t get_command_data(void)
{
    static const char *comm_char[] = {"Forward", "Right", "Left",
        "Inventory", "Look", "Eject", "Connect_nbr", "Take", "Set",
        "Incantation", "Fork", "Broadcast", "msz", "bct", "mct",
        "tna", "ppo", "plv", "pin", "sgt", "sst", NULL};
    static void (*comm_func[])(game_t *, zappy_client_t *, zappy_client_t *,
        char **) =
        {forward, right, left, inventory, look, eject,
        connect_nbr, take_object, set_object, start_incantation,
        fork_c, broadcast, command_msz, command_bct, command_mct,
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

static bool execute_graphical_command(game_t *game, zappy_client_t *user,
    zappy_client_t *clients, command_args_t *func_args)
{
    command_data_t data = get_command_data();
    char **args = str_to_word_arr(*func_args->buffer, " ");

    data.functions[func_args->cmd_index](game, user, clients, args);
    free_arr(args);
    return true;
}

static bool execute_if_free(game_t *game, zappy_client_t *user,
    zappy_client_t *clients, command_args_t *args)
{
    if (user->type == GRAPHICAL)
        return execute_graphical_command(game, user, clients,
            args);
    if (user->type == AI && user->player->busy_until <= game->current_tick){
        add_pending_cmd(user, game, args, clients);
        return true;
    } else {
        if (user->player->queue_size < 10) {
            add_to_command_queue(user, *args->buffer);
            return true;
        } else
            return true;
    }
}

static bool find_and_execute(game_t *game, zappy_client_t *user,
    zappy_client_t *clients, char *buffer)
{
    command_data_t data = get_command_data();
    command_args_t args = {0};

    for (int i = 0; data.commands[i] != NULL; i++) {
        if (strncmp(buffer, data.commands[i], strlen(data.commands[i])) == 0 &&
            user->type == data.accepted_types[i]){
            args.buffer = &buffer;
            args.cmd_index = i;
            return execute_if_free(game, user, clients, &args);
        }
    }
    return false;
}

static void send_info_new_client(game_t *game, zappy_client_t *user,
    zappy_client_t *clients)
{
    char *tmp_string = NULL;
    int len1 = snprintf(NULL, 0, "%d\n", user->client->client_id);
    int len2 = snprintf(NULL, 0, "%d %d\n",
        game->parsed_info->width,
        game->parsed_info->height);

    tmp_string = malloc(len1 + 1);
    sprintf(tmp_string, "%d\n", connect_nbr_srv(game,
        user->player->team_name));
    write_command_output_buffer(user->client, tmp_string);
    free(tmp_string);
    tmp_string = malloc(len2 + 1);
    sprintf(tmp_string, "%d %d\n",
        game->parsed_info->width,
        game->parsed_info->height);
    write_command_output_buffer(user->client, tmp_string);
    free(tmp_string);
    send_pnw_command_to_all(game, clients, user);
}

void execute_com(game_t *game, zappy_client_t *user,
    zappy_client_t *clients, char *buffer)
{
    if (!user)
        return;
    if (!user->is_fully_connected && can_connect(game, user, buffer, clients)){
        user->is_fully_connected = true;
        if (user->type == GRAPHICAL) {
            return send_map_info_to_one_client(game, clients, user);
        } else
            return send_info_new_client(game, user, clients);
    }
    if (!find_and_execute(game, user, clients, buffer)){
        if (user->is_fully_connected && user->type == GRAPHICAL)
            return write_command_output_buffer(user->client, "suc\n");
        if (user->is_fully_connected)
            write_command_output_buffer(user->client, "ko\n");
    }
}

static void check_command(int cmd_length, game_t *game,
    zappy_client_t *user, zappy_client_t *clients)
{
    char *command_str;

    if (cmd_length > MAX_CMD_LENGTH) {
        command_str = extract_command(&user->client->read_buffer, cmd_length);
        if (command_str)
            free(command_str);
        return;
    }
    command_str = extract_command(&user->client->read_buffer, cmd_length);
    if (command_str) {
        execute_com(game, user, clients, command_str);
        free(command_str);
    }
}

static void process_commands_in_buffer(game_t *game, zappy_client_t *user,
    zappy_client_t *clients)
{
    int cmd_length;

    while (true) {
        cmd_length = find_command_end(&user->client->read_buffer);
        if (cmd_length > 0)
            check_command(cmd_length, game, user, clients);
        else
            break;
    }
}

static void check_and_clear_clogged_buffer(zappy_client_t *user)
{
    if (user->client->read_buffer.count >= BUFFER_SIZE - 1 &&
        find_command_end(&user->client->read_buffer) == -1)
        init_circular_buffer(&user->client->read_buffer);
}

void get_message(server_t *server, zappy_client_t *user,
    zappy_client_t *clients, game_t *game)
{
    if (!handle_socket_read(server, &clients, user))
        return;
    process_commands_in_buffer(game, user, clients);
    check_and_clear_clogged_buffer(user);
}
