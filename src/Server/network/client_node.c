/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** client_node
*/

#include "../include/command.h"
#include <stdlib.h>

void free_node(client_t *node, server_t *server)
{
    if (node->type == GRAPHICAL)
        remove_graphic_client(server, node);
    if (node->client_poll)
        free(node->client_poll);
    if (node->client_add)
        free(node->client_add);
    if (node->player) {
        cleanup_player_queue(node->player);
        cleanup_pending(node->player);
        free(node->player);
    }
    free(node);
}
