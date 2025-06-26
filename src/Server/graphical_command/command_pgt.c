/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pgt
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_pgt_response(int client_id, resource_type_t resource_type)
{
    char *buffer = NULL;
    int size = 0;

    size = snprintf(NULL, 0, "pgt #%d %d\n", client_id, resource_type);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pgt #%d %d\n", client_id, resource_type);
    return buffer;
}

void command_pgt(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, resource_type_t resource_type)
{
    zappy_client_t *current = clients;
    char *buffer = NULL;

    (void)game;
    if (!client || !client->player || !client->client || !clients)
        return;
    buffer = format_pgt_response(client->client->client_id, resource_type);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}
