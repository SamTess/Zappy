/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** resources_map
*/

#include "../include/tile.h"
#include "../include/server.h"
#include "../include/command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

static void distribute_one_resource(resource_dist_t *dist, int res, int total,
    game_t *game, zappy_client_t *clients)
{
    int idx;
    int y;
    int x;
    int total_tiles = dist->width * dist->height;

    for (int i = 0; i < total; ++i) {
        idx = dist->tile_indices[dist->tile_idx % total_tiles];
        y = idx / dist->width;
        x = idx % dist->width;
        dist->map[y][x].resources[res]++;
        send_bct_to_all_graphical_clients(game, clients, x, y);
        dist->tile_idx++;
    }
}

static int init_resource_dist(int width, int height,
    tile_t **map, resource_dist_t *dist)
{
    int total_tiles = width * height;

    dist->tile_indices = malloc(sizeof(int) * total_tiles);
    if (dist->tile_indices == NULL)
        return -1;
    for (int i = 0; i < total_tiles; ++i)
        dist->tile_indices[i] = i;
    dist->map = map;
    dist->width = width;
    dist->height = height;
    dist->tile_idx = 0;
    return 0;
}

void distribute_resources(game_t *game, zappy_client_t *clients)
{
    resource_dist_t dist;
    int total = 0;
    int total_tiles = game->parsed_info->width * game->parsed_info->height;
    static double resource_densities[COUNT] = {FOOD_D, LINEMATE_D, DERAUMERE_D,
        SIBUR_D, MENDIANE_D, PHIRAS_D, THYSTAME_D};

    total = init_resource_dist(game->parsed_info->width,
        game->parsed_info->height, game->map, &dist);
    if (total == -1)
        return;
    shuffle_indices(dist.tile_indices, total_tiles);
    for (int res = 0; res < COUNT; ++res) {
        total = (int)(total_tiles * resource_densities[res] + 0.5);
        if (total < 1)
            total = 1;
        game->total_resources[res] = total;
        game->current_resources[res] = total;
        distribute_one_resource(&dist, res, total, game, clients);
    }
    free(dist.tile_indices);
}

void respawn_resources(game_t *game, zappy_client_t *clients)
{
    resource_dist_t dist;
    int missing = 0;
    int total_tiles = game->parsed_info->width * game->parsed_info->height;

    missing = init_resource_dist(game->parsed_info->width,
        game->parsed_info->height, game->map, &dist);
    if (missing == -1)
        return;
    shuffle_indices(dist.tile_indices, total_tiles);
    for (int res = 0; res < COUNT; ++res) {
        missing = game->total_resources[res] - game->current_resources[res];
        if (missing > 0) {
            distribute_one_resource(&dist, res, missing, game, clients);
            game->current_resources[res] += missing;
        }
    }
    send_smg_command(game, clients, "Resources respawned");
    free(dist.tile_indices);
}
