/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_sgt
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

static int get_time_unit(game_t *game)
{
    if (game && game->parsed_info)
        return game->parsed_info->frequence;
    return -1;
}

static char *format_sgt_response(int time_unit)
{
    char *buffer;
    int size;

    if (time_unit <= 0)
        return NULL;
    size = snprintf(NULL, 0, "sgt %d\n", time_unit);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "sgt %d\n", time_unit);
    return buffer;
}

void command_sgt(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    int time_unit;
    char *response;

    (void)clients;
    if (!game || !client || !client->client ||
        client->type != GRAPHICAL || arr_len(buffer) != 1) {
        write_command_output_buffer(client->client, "sbp\n");
        return;
    }
    time_unit = get_time_unit(game);
    response = format_sgt_response(time_unit);
    if (!response) {
        write_command_output_buffer(client->client, "sbp\n");
        return;
    }
    write_command_output_buffer(client->client, response);
    free(response);
}
