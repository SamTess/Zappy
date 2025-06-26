/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_bct
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static int calculate_size_bct_command(int x, int y, tile_t *tile)
{
    return snprintf(NULL, 0, "bct %d %d %d %d %d %d %d %d %d\n",
            x, y,
            tile->resources[FOOD],
            tile->resources[LINEMATE],
            tile->resources[DERAUMERE],
            tile->resources[SIBUR],
            tile->resources[MENDIANE],
            tile->resources[PHIRAS],
            tile->resources[THYSTAME]);
}

static char *get_buffer_bct_command(int x, int y, tile_t *tile)
{
    int size = calculate_size_bct_command(x, y, tile);
    char *buffer = malloc(size + 1);

    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "bct %d %d %d %d %d %d %d %d %d\n",
            x, y,
            tile->resources[FOOD],
            tile->resources[LINEMATE],
            tile->resources[DERAUMERE],
            tile->resources[SIBUR],
            tile->resources[MENDIANE],
            tile->resources[PHIRAS],
            tile->resources[THYSTAME]);
    return buffer;
}

static void send_bct_response(client_t *client, int x, int y, tile_t *tile)
{
    char *buffer = get_buffer_bct_command(x, y, tile);

    if (buffer) {
        write_command_output_buffer(client, buffer);
        free(buffer);
    }
}

void send_bct_to_all_graphical_clients(game_t *game,
    zappy_client_t *graphical_clients, int x, int y)
{
    zappy_client_t *current = graphical_clients;
    tile_t *tile;

    if (!game || !graphical_clients || !game->map || !game->parsed_info ||
        x < 0 || y < 0 || y >= game->parsed_info->height ||
        x >= game->parsed_info->width)
        return;
    tile = &game->map[y][x];
    for (; current; current = current->next) {
        if (current->type == GRAPHICAL)
            send_bct_response(current->client, x, y, tile);
    }
}

void send_mtc_to_all_graphical_clients(game_t *game,
    zappy_client_t *graphical_clients)
{
    zappy_client_t *current = graphical_clients;

    if (!game || !graphical_clients)
        return;
    for (; current; current = current->next) {
        if (current->type == GRAPHICAL)
            send_tile_content_to_one_client(game, graphical_clients, current);
    }
}

void send_tile_content_to_one_client(game_t *game, zappy_client_t *clients, zappy_client_t *client)
{
    (void)clients;
    if (!game || !client || !client->client || !game->map || !game->parsed_info)
        return;
    for (int y = 0; y < game->parsed_info->height; y++) {
        for (int x = 0; x < game->parsed_info->width; x++) {
            send_bct_response(client->client, x, y, &game->map[y][x]);
        }
    }
}

void command_bct(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    int x = 0;
    int y = 0;

    (void)clients;
    if (!game || !client || !client->client ||
        client->type != GRAPHICAL || arr_len(buffer) != 3 ||
        sscanf(buffer[1], "%d", &x) != 1 ||
        sscanf(buffer[2], "%d", &y) != 1 ||
        !game->map || !game->parsed_info || x < 0 || y < 0 ||
        y >= game->parsed_info->height ||
        x >= game->parsed_info->width)
        return write_command_output_buffer(client->client, "sbp\n");
    send_bct_response(client->client, x, y, &game->map[y][x]);
}

void command_mtc(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    (void)clients;
    if (!game || !client || !client->client ||
        client->type != GRAPHICAL || arr_len(buffer) != 1) {
        write_command_output_buffer(client->client, "sbp\n");
        return;
    }
    send_tile_content_to_one_client(game, clients, client);
}
