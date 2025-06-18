/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command_utils_bis
*/
#include "../include/command.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int check_disconnect(int bytes_read, client_t *user, server_t *server)
{
    if (bytes_read <= 0){
        remove_fd(server, user->client_fd);
        return 1;
    }
    return 0;
}

bool handle_socket_read(client_t *user, server_t *server)
{
    char temp_read_storage[1024];
    int bytes_read;

    bytes_read = read(user->client_fd,
        temp_read_storage, sizeof(temp_read_storage) - 1);
    if (check_disconnect(bytes_read, user, server) == 1)
        return false;
    for (int i = 0; i < bytes_read; i++) {
        if (add_to_circular_buffer(&user->read_buffer,
            temp_read_storage[i]) == -1)
            break;
    }
    return true;
}

static void add_type_graphic_user(client_t *user)
{
    user->type = GRAPHICAL;
    free(user->player->command_queue);
    free(user->player->pending_cmd);
    free(user->player);
    user->player = NULL;
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
        add_type_graphic_user(user);
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

bool can_connect(server_t *server, client_t *user, char *buffer)
{
    if (!is_valid_team_name(buffer, server, user)){
        write_command_output(user->client_fd, "ko\n");
        return false;
    }
    if (user->type != GRAPHICAL &&
        connect_nbr_srv(server, user->player->team_name) < 0) {
        if (user->player->team_name) {
            free(user->player->team_name);
            user->player->team_name = NULL;
        }
        write_command_output(user->client_fd, "ko\n");
        return false;
    }
    return true;
}
