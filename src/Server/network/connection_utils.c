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
#include "../include/server.h"
#include "../include/parsing.h"

int count_team(server_t *n_server)
{
    int i = 0;

    for (; n_server->parsed_info->names[i]; i++){
    }
    return i;
}

static void create_server_egg(server_t *n_server, int egg_id, int i)
{
    int pos_x = rand() % n_server->parsed_info->width;
    int pos_y = rand() % n_server->parsed_info->height;
    egg_t *n_egg = create_egg(egg_id, pos_x, pos_y,
        n_server->parsed_info->names[i]);

    if (n_egg == NULL)
        return;
    add_egg(n_server, n_egg);
}

client_t *find_client_by_socket(server_t *server, int socket_fd)
{
    client_t *temp = server->client;

    if (temp && temp->client_fd == server->s_fd)
        temp = temp->next;
    while (temp) {
        if (temp->client_fd == socket_fd)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void init_struct(client_t *new_c)
{
    init_player(new_c->player, NULL);
    if (!new_c->player)
        return;
    init_circular_buffer(&new_c->read_buffer);
    init_circular_buffer(&new_c->write_buffer);
}

void init_server_eggs(server_t *n_server)
{
    int team_count = count_team(n_server);
    int eggs_per_team = n_server->parsed_info->client_nb;
    int egg_id = 0;

    for (int i = 0; i < team_count; i++) {
        for (int j = 0; j < eggs_per_team; j++) {
            create_server_egg(n_server, egg_id, i);
            egg_id++;
        }
    }
}
