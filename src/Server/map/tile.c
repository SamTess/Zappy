/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tile
*/

#include "../include/tile.h"
#include "../include/server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void tile_init(tile_t *tile)
{
    tile->player_ids = NULL;
    tile->player_count = 0;
    tile->player_capacity = 0;
    tile->egg_ids = NULL;
    tile->egg_count = 0;
    tile->egg_capacity = 0;
    memset(tile->resources, 0, sizeof(tile->resources));
}

void tile_free(tile_t *tile)
{
    free(tile->player_ids);
    tile->player_ids = NULL;
    tile->player_count = 0;
    tile->player_capacity = 0;
    free(tile->egg_ids);
    tile->egg_ids = NULL;
    tile->egg_count = 0;
    tile->egg_capacity = 0;
}

int tile_add_player(tile_t *tile, int player_id)
{
    int new_capacity;
    int *new_ids;

    if (tile->player_count >= tile->player_capacity) {
        if (tile->player_capacity == 0)
            new_capacity = 4;
        else
            new_capacity = tile->player_capacity * 2;
        new_ids = realloc(tile->player_ids, new_capacity * sizeof(int));
        if (!new_ids)
            return -1;
        tile->player_ids = new_ids;
        tile->player_capacity = new_capacity;
    }
    tile->player_ids[tile->player_count] = player_id;
    tile->player_count++;
    return 0;
}

int tile_remove_player(tile_t *tile, int player_id)
{
    for (int i = 0; i < tile->player_count; ++i) {
        if (tile->player_ids[i] == player_id) {
            tile->player_ids[i] = tile->player_ids[tile->player_count - 1];
            tile->player_count--;
            return 0;
        }
    }
    return -1;
}
