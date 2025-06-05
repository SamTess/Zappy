/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map_creation
*/
#include "../include/tile.h"
#include "../include/server.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void create_map(server_t *server, parsing_info_t *parsed_info)
{
    server->map = malloc(sizeof(tile_t *) * parsed_info->height);
    if (server->map == NULL)
        exit(84);
    for (int y = 0; y < parsed_info->height; y++) {
        server->map[y] = malloc(sizeof(tile_t) * parsed_info->width);
        if (server->map[y] == NULL)
            exit(84);
        for (int x = 0; x < parsed_info->width; x++) {
            tile_init(&server->map[y][x]);
        }
    }
    distribute_resources(server->map, server,
        server->total_resources, server->current_resources);
}
