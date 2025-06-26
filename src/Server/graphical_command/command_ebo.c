/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_ebo
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include "../include/client.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_ebo_response(int egg_id)
{
    char *buffer;
    int size;

    size = snprintf(NULL, 0, "ebo #%d\n", egg_id);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "ebo #%d\n", egg_id);
    return buffer;
}

void send_ebo_command(game_t *game, zappy_client_t *clients, int egg_id)
{
    zappy_client_t *current = clients;
    char *buffer;

    (void)game;
    if (!clients)
        return;
    buffer = format_ebo_response(egg_id);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}
