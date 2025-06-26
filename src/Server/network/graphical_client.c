/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** graphical_client
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_message_to_all_graphic(zappy_client_t *clients, char *message)
{
    zappy_client_t *client = clients;

    while (client) {
        if (client->type == GRAPHICAL && client->client &&
            client->client->client_fd != -1)
            write_command_output_buffer(client->client, message);
        client = client->next;
    }
}

void send_map_info_to_one_client(game_t *game, zappy_client_t *clients, zappy_client_t *graphic_client)
{
    char **tmp;

    if (!game || !clients || !graphic_client)
        return;
    tmp = malloc(sizeof(char *) * 2);
    if (!tmp)
        return;
    tmp[0] = strdup("sgt\n");
    tmp[1] = NULL;
    send_msz_command(game, graphic_client);
    command_sgt(game, graphic_client, clients, tmp);
    send_tile_content_to_one_client(game, clients, graphic_client);
    send_tna_command(game, clients, graphic_client);
    send_all_player_info_to_one_client(game, clients, graphic_client);
    send_enw_command_start(game, clients);
    free_arr(tmp);
}
