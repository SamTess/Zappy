/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pie
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_pie_response(int x, int y, int result)
{
    char *buffer = NULL;
    int size = 0;

    size = snprintf(NULL, 0, "pie %d %d %d\n", x, y, result);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pie %d %d %d\n", x, y, result);
    return buffer;
}

void command_pie(game_t *game, zappy_client_t *clients, zappy_client_t *client, int result)
{
    zappy_client_t *current = clients;
    char *buffer = NULL;

    if (!game || !clients)
        return;
    buffer = format_pie_response(client->player->pos_x, client->player->pos_y, result);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}
