/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** zappy
*/
#include "include/zappy.h"
#include "include/server.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static void copy_names(game_t *game, parsing_info_t *parsed_info)
{
    int i = 0;

    for (; parsed_info->names[i] != NULL; i++){
    }
    game->parsed_info->names = calloc(i + 1, sizeof(char *));
    if (game->parsed_info->names == NULL)
        server_err("Malloc failed for parsing info names copy into server");
    for (int j = 0; parsed_info->names[j] != NULL; j++){
        game->parsed_info->names[j] = strdup(parsed_info->names[j]);
    }
    game->parsed_info->names[i] = NULL;
}

static void init_server_resources(game_t *game)
{
    game->total_resources = malloc(sizeof(int) * COUNT);
    game->current_resources = malloc(sizeof(int) * COUNT);
    if (game->total_resources == NULL || game->current_resources == NULL)
        server_err("Malloc failed for allocating resource counters");
    for (int i = 0; i < COUNT; i++) {
        game->total_resources[i] = 0;
        game->current_resources[i] = 0;
    }
}

static void create_game(game_t *game, parsing_info_t *config)
{
    game->parsed_info = calloc(1, sizeof(parsing_info_t));
    game->parsed_info->client_nb = config->client_nb;
    game->parsed_info->frequence = config->frequence;
    game->parsed_info->height = config->height;
    game->parsed_info->width = config->width;
    game->parsed_info->port = config->port;
    copy_names(game, config);
    init_server_resources(game);
    create_map(game, config);
    init_server_eggs(game);
    game->game_running = true;
    game->current_tick = 0;
}

void zappy_create(parsing_info_t *config, zappy_t *zappy)
{
    zappy->should_run = true;
    zappy->next_client_id = 0;
    zappy->clients = NULL;
    zappy->game = calloc(1, sizeof(game_t));
    create_game(zappy->game, config);
    zappy->server = calloc(1, sizeof(server_t));
    create_server(zappy->server, config, &zappy->clients);
}
