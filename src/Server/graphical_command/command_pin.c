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
    if (!buffer || !server->graphical_clients)
        return;
    write_command_output(recipient->client_fd, buffer);
    free(buffer);
}

void send_pin_to_all(server_t *server, client_t *client)
{
    graphical_client_t *current = server->graphical_clients;

    if (!server || !client || !server->graphical_clients)
        return;
    while (current) {
        send_pin_command(server, client, current->client);
        current = current->next;
    }
}

void command_pin(server_t *server, client_t *client, char *buffer)
{
    client_t *recipient = NULL;
    int id = -1;

    if (!server || !client || !server->graphical_clients)
        return;
    if (sscanf(buffer, "pin #%d\n", &id) != 1)
        return;
    if (id < 0)
        return write_command_output(client->client_fd, "ko\n");
    recipient = find_client_by_id(server, id);
    if (!recipient || recipient->type != AI) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    send_pin_command(server, recipient, client);
}
