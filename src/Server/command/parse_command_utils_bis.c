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
