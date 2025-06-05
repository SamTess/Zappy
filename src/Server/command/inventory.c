/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** inventory
*/

#include "../include/command.h"
#include "../include/player.h"
#include <stdio.h>
#include <stdlib.h>

void inventory(server_t *server, client_t *client, char *buffer)
{
    char *content;
    player_t *player = client->player;

    (void)server;
    (void)buffer;
    if (player == NULL) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    content = get_inventory_content(player);
    write_command_output(client->client_fd, content);
    free(content);
}
