/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_death
*/
#include "../include/player.h"
#include "../include/command.h"
#include "../include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void handle_player_death(game_t *game, server_t *server,
    zappy_client_t *client, zappy_client_t *clients)
{
    tile_t *tile;

    if (!client || !client->player)
        return;
    write_command_output(client->client->client_fd, "dead\n");
    command_pdi(game, client, clients);
    if (game->map && client->player->pos_y >= 0 &&
        client->player->pos_y < game->parsed_info->height &&
        client->player->pos_x >= 0 &&
        client->player->pos_x < game->parsed_info->width) {
        tile = &game->map[client->player->pos_y][client->player->pos_x];
        tile_remove_player(tile, client->client->client_id);
    }
    remove_fd(server, &clients, client->client->client_fd);
}

bool check_player_starvation(game_t *game, server_t *server,
    zappy_client_t *client, zappy_client_t *clients)
{
    int food_amount = 0;
    bool dead = false;

    if (!client || !client->player || !client->is_fully_connected)
        return dead;
    client->player->life--;
    if (client->player->life <= 0) {
        food_amount = how_many_in_inventory(client->player, FOOD);
        if (food_amount > 0) {
            remove_item_from_inventory(client->player, FOOD, 1);
            client->player->life = MAX_LIFE_AFTER_FOOD;
        } else {
            handle_player_death(game, server, client, clients);
            dead = true;
        }
    }
    return dead;
}
