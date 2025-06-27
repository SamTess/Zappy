/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** map_creation
*/
#include "../include/tile.h"
#include "../include/game.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void create_map(game_t *game, parsing_info_t *parsed_info)
{
    game->map = malloc(sizeof(tile_t *) * parsed_info->height);
    if (game->map == NULL)
        parsing_error("Map allocation failed", parsed_info);
    for (int y = 0; y < parsed_info->height; y++) {
        game->map[y] = malloc(sizeof(tile_t) * parsed_info->width);
        if (game->map[y] == NULL)
            parsing_error("Tile allocation failed", parsed_info);
        for (int x = 0; x < parsed_info->width; x++) {
            tile_init(&game->map[y][x]);
        }
    }
    distribute_resources(game, NULL);
}
