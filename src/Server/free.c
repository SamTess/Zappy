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

static void free_client(server_t *server)
{
    client_t *current_client = server->client;
    client_t *next_client;

    if (server->client->next != NULL)
        current_client = server->client->next;
    while (current_client != NULL) {
        next_client = current_client->next;
        free_node(current_client, server);
        current_client = next_client;
    }
    server->client = NULL;
}

static void free_eggs(server_t *server)
{
    egg_t *current_egg = server->eggs;
    egg_t *next_egg;

    while (current_egg != NULL) {
        next_egg = current_egg->next;
        free(current_egg->team_name);
        current_egg->team_name = NULL;
        free(current_egg);
        current_egg = next_egg;
    }
    server->eggs = NULL;
}

static void free_tiles(server_t *server, int i, parsing_info_t *parsed_info)
{
    if (!server || !server->map || !server->map[i] || !parsed_info)
        return;
    for (int j = 0; j < parsed_info->width; j++) {
        tile_free(&server->map[i][j]);
    }
    free(server->map[i]);
    server->map[i] = NULL;
}

static void free_map(server_t *server, parsing_info_t *parsed_info)
{
    if (server->map && parsed_info && parsed_info->height > 0
        && parsed_info->width > 0) {
        for (int i = 0; i < parsed_info->height; i++) {
            free_tiles(server, i, parsed_info);
        }
        free(server->map);
        server->map = NULL;
    }
    free(server->serv_add);
    server->serv_add = NULL;
    free(server->total_resources);
    server->total_resources = NULL;
    free(server->current_resources);
    server->current_resources = NULL;
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
    if (server->poll_manager->fds)
        free(server->poll_manager->fds);
    free(server->poll_manager);
}

void free_all(server_t *server, parsing_info_t *parsed_info)
{
    if (!server)
        return;
    free_client(server);
    free_eggs(server);
    free_map(server, parsed_info);
    if (server->poll_manager)
        free_poll_mana(server);
    if (parsed_info)
        free_parsed_info(parsed_info);
    if (server->parsed_info) {
        free_parsed_info(server->parsed_info);
        free(server->parsed_info);
        server->parsed_info = NULL;
    }
}
