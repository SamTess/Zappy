/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** game
*/

#ifndef GAME_H_
    #define GAME_H_
    #include "parsing.h"
    #include "tile.h"
    #include "egg.h"
    #include "player.h"

typedef struct game_s {
    egg_t *eggs;
    tile_t **map;
    int current_tick;
    parsing_info_t *parsed_info;
    int *total_resources;
    int *current_resources;
    int next_egg_id;
    bool game_running;
} game_t;

#endif /* !GAME_H_ */
