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

void handle_player_death(server_t *server, client_t *client)
{
    tile_t *tile;

    if (!client || !client->player)
        return;
    write_command_output(client->client_fd, "dead\n");
    if (server->map && client->player->pos_y < server->parsed_info->height &&
        client->player->pos_x < server->parsed_info->width) {
        tile = &server->map[client->player->pos_y][client->player->pos_x];
        tile_remove_player(tile, client->client_id);
    }
    if (client->player) {
        free_inventory(client->player);
        cleanup_player_queue(client->player);
        free(client->player->team_name);
        free(client->player);
        client->player = NULL;
    }
    close(client->client_fd);
    remove_fd(server, client->client_fd);
}

void check_player_starvation(server_t *server, client_t *client)
{
    int food_amount = 0;

    if (!client || !client->player)
        return;
    client->player->life--;
    if (client->player->life <= 0) {
        food_amount = how_many_in_inventory(client->player, FOOD);
        if (food_amount > 0) {
            remove_item_from_inventory(client->player, FOOD, 1);
            client->player->life = MAX_LIFE_AFTER_FOOD;
        } else
            handle_player_death(server, client);
    }
}
