/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_init
*/

#include "../include/player.h"
#include "../include/server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_new_player_pos(server_t *server, client_t *new_client)
{
    int random_x = rand() % server->parsed_info->width;
    int random_y = rand() % server->parsed_info->height;

    new_client->player->pos_x = random_x;
    new_client->player->pos_y = random_y;
    tile_add_player(&server->map[random_y][random_x], new_client->client_id);
    printf("New player from client with id %d was spawned at position x: %d and y: %d\n", new_client->client_id, random_x, random_y);
}

void init_player(player_t *player, char *playerTeam)
{
    player->pos_x = 0;
    player->pos_y = 0;
    player->rotation = UP;
    player->busy_until = 0;
    player->queue_size = 0;
    player->command_queue = calloc(10, sizeof(char *));
    if (player->command_queue == NULL)
        malloc_failed(5);
    player->level = 1;
    player->life = 126;
    if (playerTeam != NULL) {
        player->team_name = strdup(playerTeam);
        if (player->team_name == NULL)
            malloc_failed(4);
    } else
        player->team_name = NULL;
    init_inventory(player);
}
