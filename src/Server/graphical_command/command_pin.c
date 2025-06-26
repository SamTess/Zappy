/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pin
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int calculate_size_pin_command(zappy_client_t *client)
{
    if (!client || !client->player || !client->client)
        return 0;
    return snprintf(NULL, 0, "pin #%d %d %d %d %d %d %d %d %d %d\n",
        client->client->client_id,
        client->player->pos_x,
        client->player->pos_y,
        client->player->inventory[FOOD],
        client->player->inventory[LINEMATE],
        client->player->inventory[DERAUMERE],
        client->player->inventory[SIBUR],
        client->player->inventory[MENDIANE],
        client->player->inventory[PHIRAS],
        client->player->inventory[THYSTAME]);
}

static char *format_pin_response(zappy_client_t *client)
{
    int size = calculate_size_pin_command(client);
    char *buffer = malloc(size + 1);

    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pin #%d %d %d %d %d %d %d %d %d %d\n",
            client->client->client_id,
            client->player->pos_x,
            client->player->pos_y,
            client->player->inventory[FOOD],
            client->player->inventory[LINEMATE],
            client->player->inventory[DERAUMERE],
            client->player->inventory[SIBUR],
            client->player->inventory[MENDIANE],
            client->player->inventory[PHIRAS],
            client->player->inventory[THYSTAME]);
    return buffer;
}

void send_pin_command(game_t *game, zappy_client_t *clients, zappy_client_t *sender, zappy_client_t *recipient)
{
    char *buffer;

    (void)game;
    (void)clients;
    if (!sender || !sender->player || !sender->client ||
        !recipient || !recipient->client)
        return;
    buffer = format_pin_response(sender);
    if (buffer) {
        write_command_output_buffer(recipient->client, buffer);
        free(buffer);
    }
}

void send_pin_to_all(game_t *game, zappy_client_t *clients, zappy_client_t *sender)
{
    zappy_client_t *current = clients;

    if (!game || !sender || !sender->player || !clients)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client && current->is_fully_connected)
            send_pin_command(game, clients, sender, current);
        current = current->next;
    }
}

void command_pin(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer)
{
    zappy_client_t *target = NULL;
    int id = -1;

    if (!game || !client || !client->client || !clients ||
        client->type != GRAPHICAL || arr_len(buffer) != 2 ||
        sscanf(buffer[1], "#%d", &id) != 1 || id < 0)
        return write_command_output_buffer(client->client, "sbp\n");
    target = find_client_by_id(clients, id);
    if (!target || target->type != AI) {
        write_command_output_buffer(client->client, "sbp\n");
        return;
    }
    send_pin_command(game, clients, target, client);
}
