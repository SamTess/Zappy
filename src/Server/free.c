/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** free
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/server.h"
#include "../include/game.h"
#include "../include/zappy.h"

static void free_client(zappy_client_t *clients)
{
    zappy_client_t *current_client = clients;
    zappy_client_t *next_client;

    while (current_client != NULL) {
        next_client = current_client->next;
        free_node(current_client);
        current_client = next_client;
    }
}

static void free_eggs(game_t *game)
{
    egg_t *current_egg = game->eggs;
    egg_t *next_egg;

    while (current_egg != NULL) {
        next_egg = current_egg->next;
        free(current_egg->team_name);
        free(current_egg);
        current_egg = next_egg;
    }
    game->eggs = NULL;
}

static void free_tiles(game_t *game, int i, parsing_info_t *parsed_info)
{
    if (!game || !game->map || !game->map[i] || !parsed_info)
        return;
    for (int j = 0; j < parsed_info->width; j++) {
        tile_free(&game->map[i][j]);
    }
    free(game->map[i]);
    game->map[i] = NULL;
}

static void free_map(game_t *game, parsing_info_t *parsed_info)
{
    if (game->map && parsed_info && parsed_info->height > 0
        && parsed_info->width > 0) {
        for (int i = 0; i < parsed_info->height; i++) {
            free_tiles(game, i, parsed_info);
        }
        free(game->map);
        game->map = NULL;
    }
    free(game->total_resources);
    game->total_resources = NULL;
    free(game->current_resources);
    game->current_resources = NULL;
}

static void free_parsed_info(parsing_info_t *p_info)
{
    if (!p_info || !p_info->names)
        return;
    for (int i = 0; p_info->names[i] != NULL; i++) {
        free(p_info->names[i]);
        p_info->names[i] = NULL;
    }
    free(p_info->names);
    p_info->names = NULL;
}

static void free_poll_mana(server_t *server)
{
    if (server->poll_manager && server->poll_manager->fds)
        free(server->poll_manager->fds);
    if (server->poll_manager)
        free(server->poll_manager);
}

void free_all(server_t *server, game_t *game, zappy_client_t *clients,
    parsing_info_t *parsed_info)
{
    if (!server || !game)
        return;
    free_client(clients);
    free_eggs(game);
    free_map(game, parsed_info);
    if (server->poll_manager)
        free_poll_mana(server);
    if (parsed_info)
        free_parsed_info(parsed_info);
    if (game->parsed_info){
        free_parsed_info(game->parsed_info);
        free(game->parsed_info);
    }
    free(server->serv_add);
}
