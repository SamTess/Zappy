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

void command_pic(game_t *game, zappy_client_t *clients, int x, int y, int level)
{
    zappy_client_t *current = clients;
    char *buffer = NULL;
    tile_t *tile = NULL;

    if (!game || !clients || !game->map || !game->parsed_info)
        return;
    if (y < 0 || x < 0 || y >= game->parsed_info->height ||
        x >= game->parsed_info->width)
        return;
    tile = &game->map[y][x];
    buffer = format_pic_response(x, y, level, tile);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}
