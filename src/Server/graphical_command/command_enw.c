/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_enw
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static char *format_enw_response(int egg_id, int client_id,
    int pos_x, int pos_y)
{
    char *buffer = NULL;
    int size = snprintf(NULL, 0, "enw #%d #%d %d %d\n",
            egg_id, client_id, pos_x, pos_y);

    buffer = malloc(size + 1);
    if (buffer) {
        snprintf(buffer, size + 1, "enw #%d #%d %d %d\n",
                egg_id, client_id, pos_x, pos_y);
    }
    return buffer;
}

void send_enw_command(game_t *game, zappy_client_t *clients,
    zappy_client_t *parent, int egg_id)
{
    char *buffer = NULL;
    zappy_client_t *current = clients;

    (void)game;
    if (!clients || !parent || !parent->player || !parent->client)
        return;
    buffer = format_enw_response(egg_id, parent->client->client_id,
        parent->player->pos_x, parent->player->pos_y);
    if (!buffer)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
    free(buffer);
}

static void send_enw_command_to_client(zappy_client_t *recipient, egg_t *egg)
{
    char *buffer = NULL;

    if (!recipient || !recipient->client || !egg)
        return;
    buffer = format_enw_response(egg->egg_id, -1, egg->pos_x, egg->pos_y);
    if (buffer) {
        write_command_output_buffer(recipient->client, buffer);
        free(buffer);
    }
}

void send_enw_command_start(game_t *game, zappy_client_t *clients)
{
    zappy_client_t *current;
    egg_t *egg;

    if (!game || !clients || !game->eggs)
        return;
    egg = game->eggs;
    while (egg) {
        current = clients;
        while (current) {
            if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
                send_enw_command_to_client(current, egg);
            current = current->next;
        }
        egg = egg->next;
    }
}
