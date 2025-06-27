/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** inventory
*/

#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/player.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

void inventory(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer)
{
    char *content;

    (void)game;
    (void)clients;
    if (!client || !client->client || !client->player || arr_len(buffer) != 1)
        return write_command_output_buffer(client->client, "ko\n");
    content = get_inventory_content(client->player);
    write_command_output_buffer(client->client, content);
    free(content);
}
