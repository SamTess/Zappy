/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_edi
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_edi_response(int egg_id)
{
    char *buffer = NULL;
    int size = 0;

    size = snprintf(NULL, 0, "edi #%d\n", egg_id);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "edi #%d\n", egg_id);
    return buffer;
}

void send_edi_command(game_t *game, zappy_client_t *clients, int egg_id)
{
    char *buffer = NULL;
    zappy_client_t *current = clients;

    (void)game;
    if (!clients)
        return;
    buffer = format_edi_response(egg_id);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client
            && current->is_fully_connected && current->client->client_id != -1)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}
