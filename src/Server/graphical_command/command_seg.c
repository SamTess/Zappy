/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_seg
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_seg_response(char *team_name)
{
    char *buffer = NULL;
    int size = 0;

    if (!team_name)
        return NULL;
    size = snprintf(NULL, 0, "seg %s\n", team_name);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "seg %s\n", team_name);
    return buffer;
}

void command_seg(game_t *game, zappy_client_t *clients, char *team_name)
{
    zappy_client_t *current = clients;
    char *buffer = NULL;

    (void)game;
    if (!clients || !team_name)
        return;
    buffer = format_seg_response(team_name);
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
