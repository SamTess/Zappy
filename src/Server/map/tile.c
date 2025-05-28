/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tile
*/

#include "../include/tile.h"
#include <stdlib.h>
#include <string.h>

void tile_init(tile_t *tile)
{
    tile->player_ids = NULL;
    tile->player_count = 0;
    tile->player_capacity = 0;
    memset(tile->resources, 0, sizeof(tile->resources));
}

void tile_free(tile_t *tile)
{
    free(tile->player_ids);
    tile->player_ids = NULL;
    tile->player_count = 0;
    tile->player_capacity = 0;
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

static void shuffle_indices(int *indices, int total_tiles)
{
    int j;
    int tmp;

    for (int i = total_tiles - 1; i > 0; --i) {
        j = rand() % (i + 1);
        tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
}

static void distribute_one_resource(resource_dist_t *dist, int res, int total)
{
    int idx;
    int y;
    int x;

    shuffle_indices(dist->tile_indices, dist->width * dist->height);
    for (int i = 0; i < total; ++i) {
        idx = dist->tile_indices[i];
        y = idx / dist->width;
        x = idx % dist->width;
        dist->map[y][x].resources[res]++;
    }
}

void distribute_resources(tile_t **map, int width, int height)
{
    int total_tiles;
    int total;
    resource_dist_t dist;
    static double resource_densities[COUNT] = {FOOD_D, LINEMATE_D, DERAUMERE_D,
        SIBUR_D, MENDIANE_D, PHIRAS_D, THYSTAME_D};

    total_tiles = width * height;
    dist.tile_indices = malloc(sizeof(int) * total_tiles);
    for (int i = 0; i < total_tiles; ++i)
        dist.tile_indices[i] = i;
    dist.map = map;
    dist.width = width;
    dist.height = height;
    for (int res = 0; res < COUNT; ++res) {
        total = (int)(width * height * resource_densities[res] + 0.5);
        if (total < 1)
            total = 1;
        distribute_one_resource(&dist, res, total);
    }
    free(dist.tile_indices);
}
