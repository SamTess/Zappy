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

static void distribute_one_resource(resource_dist_t *dist, int res,
    game_t *game, zappy_client_t *clients)
{
    int idx;
    int y;
    int x;
    int total_tiles = dist->width * dist->height;
    int tiles_used = 0;

    shuffle_indices(dist->tile_indices, total_tiles);
    for (int i = 0; i < dist->total; ++i) {
        if (tiles_used >= total_tiles) {
            shuffle_indices(dist->tile_indices, total_tiles);
            tiles_used = 0;
        }
        idx = dist->tile_indices[i % total_tiles];
        y = idx / dist->width;
        x = idx % dist->width;
        dist->map[y][x].resources[res]++;
        send_bct_to_all_graphical_clients(game, clients, x, y);
        tiles_used++;
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
    dist->total = 0;
    return 0;
}

void distribute_resources(game_t *game, zappy_client_t *clients)
{
    resource_dist_t dist;
    int total_tiles = game->parsed_info->width * game->parsed_info->height;
    static double resource_densities[COUNT] = {FOOD_D, LINEMATE_D, DERAUMERE_D,
        SIBUR_D, MENDIANE_D, PHIRAS_D, THYSTAME_D};

    if (init_resource_dist(game->parsed_info->width,
        game->parsed_info->height, game->map, &dist) == -1)
        return;
    for (int res = 0; res < COUNT; ++res) {
        dist.total = (int)(total_tiles * resource_densities[res] + 0.5);
        if (dist.total < 1)
            dist.total = 1;
        game->total_resources[res] = dist.total;
        game->current_resources[res] = dist.total;
        distribute_one_resource(&dist, res, game, clients);
    }
    free(dist.tile_indices);
}

void respawn_resources(game_t *game, zappy_client_t *clients)
{
    resource_dist_t dist;

    if (init_resource_dist(game->parsed_info->width,
        game->parsed_info->height, game->map, &dist) == -1)
        return;
    for (int res = 0; res < COUNT; ++res) {
        dist.total = game->total_resources[res] - game->current_resources[res];
        if (dist.total > 0) {
            distribute_one_resource(&dist, res, game, clients);
            game->current_resources[res] += dist.total;
        }
    }
    send_smg_command(game, clients, "Resources respawned");
    free(dist.tile_indices);
}
