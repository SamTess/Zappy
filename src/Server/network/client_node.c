/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** client_node
*/

#include "../include/command.h"
#include <stdlib.h>

static void cleanup_player_client(client_t *current_client)
{
    player_t *player = current_client->player;

    free(player->team_name);
    player->team_name = NULL;
    free_inventory(player);
    if (player->command_queue)
        cleanup_player_queue(player);
    if (player->pending_cmd){
        if (player->pending_cmd->args){
            free(player->pending_cmd->args);
            player->pending_cmd->args = NULL;
        }
        free(player->pending_cmd);
        player->pending_cmd = NULL;
    }
    free(player);
    current_client->player = NULL;
}

void free_node(client_t *node, server_t *server)
{
    if (node->type == GRAPHICAL)
        remove_graphic_client(server, node);
    if (node->client_poll)
        free(node->client_poll);
    if (node->client_add)
        free(node->client_add);
    if (node->player) {
        cleanup_player_client(node);
        cleanup_pending(node->player);
        free(node->player);
    }
    free(node);
}
