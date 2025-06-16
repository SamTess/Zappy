/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** resources_map
*/

#include "../include/tile.h"
#include "../include/server.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
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
    int total, server_t *server)
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
        send_bct_to_all_graphical_clients(server, x, y);
    }
}

static void init_resource_dist(int width, int height,
    tile_t **map, resource_dist_t *dist)
{
    int total_tiles = width * height;

    dist->tile_indices = malloc(sizeof(int) * total_tiles);
    if (dist->tile_indices == NULL)
        exit(84);
    for (int i = 0; i < total_tiles; ++i)
        dist->tile_indices[i] = i;
    dist->map = map;
    dist->width = width;
    dist->height = height;
}

void distribute_resources(tile_t **map, server_t *server,
    int *total_resources, int *current_resources)
{
    int total;
    resource_dist_t dist;
    static double resource_densities[COUNT] = {FOOD_D, LINEMATE_D, DERAUMERE_D,
        SIBUR_D, MENDIANE_D, PHIRAS_D, THYSTAME_D};

    init_resource_dist(server->parsed_info->width,
        server->parsed_info->height, map, &dist);
    for (int res = 0; res < COUNT; ++res) {
        total = (int)(server->parsed_info->width *
            server->parsed_info->height * resource_densities[res] + 0.5);
        if (total < 1)
            total = 1;
        total_resources[res] = total;
        current_resources[res] = total;
        distribute_one_resource(&dist, res, total, server);
    }
    free(dist.tile_indices);
}

void respawn_resources(tile_t **map, server_t *server,
    int *total_resources, int *current_resources)
{
    resource_dist_t dist;
    int missing;

    init_resource_dist(server->parsed_info->width,
        server->parsed_info->height, map, &dist);
    for (int res = 0; res < COUNT; ++res) {
        missing = total_resources[res] - current_resources[res];
        if (missing > 0) {
            distribute_one_resource(&dist, res, missing, server);
            current_resources[res] += missing;
        }
    }
    send_smg_command(server, "Resources respawned");
    free(dist.tile_indices);
}
