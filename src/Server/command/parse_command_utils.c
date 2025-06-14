/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command_utils
*/
#include "../include/command.h"
#include "../include/parsing.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

void cleanup_pending(player_t *player)
{
    if (!player || !player->pending_cmd)
        return;
    if (player->pending_cmd->args){
        free(player->pending_cmd->args);
        player->pending_cmd->args = NULL;
    }
    free(player->pending_cmd);
    player->pending_cmd = NULL;
}

void add_pending_cmd(client_t *user, server_t *server,
    char *buffer, int cmd_index)
{
    command_data_t data = get_command_data();
    char **tmp = NULL;

    if (cmd_index == 9) {
        tmp = str_to_word_arr(buffer, " ");
        start_incantation(server, user, tmp);
        return free_arr(tmp);
    }
    if (cmd_index == 10)
        command_pfk(server, user);
    user->player->pending_cmd->args = strdup(buffer);
    user->player->pending_cmd->func = data.functions[cmd_index];
    if (data.times[cmd_index] > 0)
        user->player->busy_until =
            server->current_tick + data.times[cmd_index];
}

void cleanup_player_queue(player_t *player)
{
    if (!player || !player->command_queue)
        return;
    for (int i = 0; i < player->queue_size; i++) {
        if (player->command_queue[i]) {
            free(player->command_queue[i]);
            player->command_queue[i] = NULL;
        }
    }
    free(player->command_queue);
    player->command_queue = NULL;
    player->queue_size = 0;
}

void cleanup_client(client_t *client)
{
    if (!client)
        return;
    if (client->player) {
        cleanup_player_queue(client->player);
        cleanup_pending(client->player);
        free(client->player);
        client->player = NULL;
    }
}

void write_command_output(int client_fd, char *msg)
{
    if (fcntl(client_fd, F_GETFD) == -1) {
        perror("FD isn't up anymore\n");
    } else {
        write(client_fd, msg, strlen(msg));
    }
}
