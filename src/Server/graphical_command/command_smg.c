/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_smg
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *format_smg_response(char *msg)
{
    char *buffer = NULL;
    int size = 0;

    if (!msg)
        return NULL;
    size = snprintf(NULL, 0, "smg %s\n", msg);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "smg %s\n", msg);
    return buffer;
}

void send_smg_command(game_t *game, zappy_client_t *clients, char *msg)
{
    zappy_client_t *current = clients;
    char *buffer = NULL;

    (void)game;
    if (!clients || !msg)
        return;
    buffer = format_smg_response(msg);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}
