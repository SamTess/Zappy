/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** client_node
*/

#include "../include/server.h"
#include <stdlib.h>

void free_node(client_t *node)
{
    if (node->client_poll)
        free(node->client_poll);
    if (node->client_add)
        free(node->client_add);
    if (node->player)
        free(node->player);
    free(node);
}
