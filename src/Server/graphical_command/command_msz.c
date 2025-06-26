/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_msz
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

void send_msz_command(game_t *game, zappy_client_t *client)
{
    char *buffer = NULL;
    int size = 0;

    if (!game || !client)
        return;
    buffer = NULL;
    size = snprintf(NULL, 0, "msz %d %d\n",
        game->parsed_info->width, game->parsed_info->height);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "msz %d %d\n",
            game->parsed_info->width, game->parsed_info->height);
    write_command_output_buffer(client->client, buffer);
    free(buffer);
}

void command_msz(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    (void)clients;
    if (!game || !client || arr_len(buffer) != 1)
        return write_command_output_buffer(client->client, "sbp\n");
    send_msz_command(game, client);
}
