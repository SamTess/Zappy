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

static int count_team(server_t *n_server)
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
        server_err("Error in egg creation");
    add_egg(n_server, n_egg);
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
