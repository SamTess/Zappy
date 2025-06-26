/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_tna
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

static void send_one_tna_command(game_t *game, client_t *client,
    const char *team)
{
    int size = 0;
    char *buffer = NULL;

    if (!game || !client || !team)
        return;
    size = snprintf(NULL, 0, "tna %s\n", team);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "tna %s\n", team);
    write_command_output_buffer(client, buffer);
    free(buffer);
}

void send_tna_command(game_t *game, zappy_client_t *clients, zappy_client_t *client)
{
    (void)clients;
    if (!game || !client)
        return;
    for (int i = 0; game->parsed_info->names[i]; i++) {
        send_one_tna_command(game, client->client, game->parsed_info->names[i]);
    }
}

void command_tna(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    if (!game || !client || !buffer || !game->parsed_info ||
        !game->parsed_info->names || arr_len(buffer) != 1)
        return write_command_output_buffer(client->client, "sbp\n");
    send_tna_command(game, clients, client);
}
