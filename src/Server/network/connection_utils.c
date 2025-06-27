/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** connection_utils
*/
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <linux/limits.h>
#include "../include/zappy.h"
#include "../include/server.h"
#include "../include/parsing.h"

int count_team(game_t *game)
{
    int i = 0;

    for (; game->parsed_info->names[i]; i++){
    }
    return i;
}

int init_zappy_client_struct(zappy_client_t *new_c)
{
    new_c->client = calloc(1, sizeof(client_t));
    if (!new_c->client)
        return -1;
    new_c->client->client_poll = calloc(1, sizeof(struct pollfd));
    if (!new_c->client->client_poll)
        return -1;
    return 0;
}

static void create_server_egg(game_t *n_game, int egg_id, int i)
{
    int pos_x = rand() % n_game->parsed_info->width;
    int pos_y = rand() % n_game->parsed_info->height;
    egg_t *n_egg = create_egg(egg_id, pos_x, pos_y,
        n_game->parsed_info->names[i]);

    if (n_egg == NULL)
        return;
    add_egg(n_game, n_egg);
}

zappy_client_t *find_client_by_socket(zappy_client_t *clients,
    server_t *server, int socket_fd)
{
    zappy_client_t *temp = clients;

    if (temp && temp->client->client_fd == server->s_fd)
        temp = temp->next;
    while (temp) {
        if (temp->client->client_fd == socket_fd)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void init_struct(zappy_client_t *new_c)
{
    init_player(new_c->player, NULL);
    if (!new_c->player)
        return;
    init_circular_buffer(&new_c->client->read_buffer);
    init_circular_buffer(&new_c->client->write_buffer);
}

void init_server_eggs(game_t *n_game)
{
    int team_count = count_team(n_game);
    int eggs_per_team = n_game->parsed_info->client_nb;
    int egg_id = 0;

    for (int i = 0; i < team_count; i++) {
        for (int j = 0; j < eggs_per_team; j++) {
            create_server_egg(n_game, egg_id, i);
            egg_id++;
        }
    }
    n_game->next_egg_id = egg_id;
}
