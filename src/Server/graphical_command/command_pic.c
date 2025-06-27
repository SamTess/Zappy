/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pic
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_pic_response(int x, int y, int level, tile_t *tile)
{
    int size = 0;
    char *buffer = NULL;
    int offset = 0;

    size = snprintf(NULL, 0, "pic %d %d %d", x, y, level);
    for (int i = 0; i < tile->player_count; i++)
        size += snprintf(NULL, 0, " #%d", tile->player_ids[i]);
    size += 2;
    buffer = malloc(size);
    if (!buffer)
        return NULL;
    offset = snprintf(buffer, size, "pic %d %d %d", x, y, level);
    for (int i = 0; i < tile->player_count; i++)
        offset += snprintf(buffer + offset, size - offset, " #%d",
            tile->player_ids[i]);
    snprintf(buffer + offset, size - offset, "\n");
    return buffer;
}

void command_pic(game_t *game, zappy_client_t *clients,
    int *coords, int level)
{
    zappy_client_t *current = clients;
    char *buffer = NULL;
    tile_t *tile = NULL;

    if (!game || !clients || !game->map || !game->parsed_info)
        return;
    if (coords[1] < 0 || coords[0] < 0 || coords[1] >=
        game->parsed_info->height || coords[0] >= game->parsed_info->width)
        return;
    tile = &game->map[coords[1]][coords[0]];
    buffer = format_pic_response(coords[0], coords[1], level, tile);
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
