/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pex
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_pex_response(int client_id)
{
    char *buffer = NULL;
    int size = 0;

    size = snprintf(NULL, 0, "pex #%d\n", client_id);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pex #%d\n", client_id);
    return buffer;
}

void command_pex(game_t *game, zappy_client_t *client, zappy_client_t *clients)
{
    zappy_client_t *current = clients;
    char *buffer = NULL;

    (void)game;
    if (!client || !client->player || !client->client || !clients)
        return;
    buffer = format_pex_response(client->client->client_id);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}
