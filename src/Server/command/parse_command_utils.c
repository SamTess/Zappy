/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command_utils
*/
#include "../include/command.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static void cleanup_pending(player_t *player)
{
    if (!player || !player->pending_cmd)
        return;
    free(player->pending_cmd->args);
    free(player->pending_cmd);
    player->pending_cmd = NULL;
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
    write(client_fd, msg, strlen(msg));
}
