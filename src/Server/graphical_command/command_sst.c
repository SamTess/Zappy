/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_sst
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char *format_sst_response(int time)
{
    char *buffer = NULL;
    int size = snprintf(NULL, 0, "sst %d\n", time);

    buffer = malloc(size + 1);
    if (!buffer) {
        return NULL;
    }
    snprintf(buffer, size + 1, "sst %d\n", time);
    return buffer;
}

static int get_time_from_buffer(char *buffer)
{
    int time = 0;

    if (sscanf(buffer, "%d", &time) != 1 || time <= 0) {
        return -1;
    }
    return time;
}

static void send_command(zappy_client_t *clients, char *response)
{
    zappy_client_t *current = clients;

    while (current) {
        if (current->type == GRAPHICAL && current->client
            && current->is_fully_connected && current->client->client_id != -1)
            write_command_output_buffer(current->client, response);
        current = current->next;
    }
}

void command_sst(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer)
{
    int time;
    char *response = NULL;

    if (!game || !client || !client->client || !clients || !buffer ||
        client->type != GRAPHICAL || arr_len(buffer) != 2)
        return write_command_output_buffer(client->client, "sbp\n");
    time = get_time_from_buffer(buffer[1]);
    if (time <= 0)
        return write_command_output_buffer(client->client, "sbp\n");
    response = format_sst_response(time);
    if (!response)
        return write_command_output_buffer(client->client, "sbp\n");
    game->parsed_info->frequence = time;
    send_command(clients, response);
    free(response);
}
