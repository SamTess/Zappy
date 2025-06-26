/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** client_node
*/

#include "../include/command.h"
#include <stdlib.h>

static void free_player_queue(player_t *player)
{
    for (int i = 0; i < 10; i++){
        if (player->command_queue[i]){
            free(player->command_queue[i]);
            player->command_queue[i] = NULL;
        }
    }
    free(player->command_queue);
    player->command_queue = NULL;
}

static void cleanup_player_client(zappy_client_t *current_client)
{
    player_t *player;

    if (!current_client || !current_client->player)
        return;
    player = current_client->player;
    if (player->team_name){
        free(player->team_name);
        player->team_name = NULL;
    }
    if (player->command_queue)
        free_player_queue(current_client->player);
    if (player->pending_cmd){
        if (player->pending_cmd->args){
            free(player->pending_cmd->args);
            player->pending_cmd->args = NULL;
        }
        free(player->pending_cmd);
        player->pending_cmd = NULL;
    }
}

void free_node(zappy_client_t *node)
{
    if (!node)
        return;
    if (node->client->client_poll) {
        free(node->client->client_poll);
        node->client->client_poll = NULL;
    }
    if (node->type == AI && node->player) {
        cleanup_player_client(node);
        free(node->player);
        node->player = NULL;
    }
    free(node);
}
