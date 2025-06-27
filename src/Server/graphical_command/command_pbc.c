/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pbc
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_pbc_response(int client_id, char *message)
{
    char *buffer = NULL;
    int size = 0;

    if (!message)
        return NULL;
    size = snprintf(NULL, 0, "pbc #%d %s", client_id, message);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pbc #%d %s", client_id, message);
    return buffer;
}

void command_pbc(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char *message)
{
    zappy_client_t *current = NULL;
    char *response = NULL;

    if (!game || !client || !client->client
        || !client->player || !clients || !message)
        return;
    response = format_pbc_response(client->client->client_id, message);
    if (!response)
        return;
    current = clients;
    for (; current != NULL; current = current->next) {
        if (current->type == GRAPHICAL && current->client->client_id != -1)
            write_command_output_buffer(current->client, response);
    }
    free(response);
}
