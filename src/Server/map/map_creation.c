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
    for (int y = 0; y < parsed_info->height; y++) {
        server->map[y] = malloc(sizeof(tile_t) * parsed_info->width);
        for (int x = 0; x < parsed_info->width; x++) {
            tile_init(&server->map[y][x]);
        }
    }
    distribute_resources(server->map, parsed_info->width, parsed_info->height);
}
