/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pin
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

static int calculate_size_pin_command(client_t *client)
{
    if (!client || !client->player)
        return 0;
    return snprintf(NULL, 0, "pin #%d %d %d %d %d %d %d %d %d %d\n",
        client->client_id,
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

static char *get_buffer_pin_command(client_t *client)
{
    int size = calculate_size_pin_command(client);
    char *buffer = malloc(size + 1);

    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pin #%d %d %d %d %d %d %d %d %d %d\n",
            client->client_id,
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

void send_pin_command(server_t *server, client_t *client, client_t *recipient)
{
    char *buffer = get_buffer_pin_command(client);

    (void)server;
    if (!buffer)
        return;
    write_command_output(recipient->client_fd, buffer);
    free(buffer);
}
