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
                client->player->inventory[FOOD].quantity,
                client->player->inventory[LINEMATE].quantity,
                client->player->inventory[DERAUMERE].quantity,
                client->player->inventory[SIBUR].quantity,
                client->player->inventory[MENDIANE].quantity,
                client->player->inventory[PHIRAS].quantity,
                client->player->inventory[THYSTAME].quantity);
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
            client->player->inventory[FOOD].quantity,
            client->player->inventory[LINEMATE].quantity,
            client->player->inventory[DERAUMERE].quantity,
            client->player->inventory[SIBUR].quantity,
            client->player->inventory[MENDIANE].quantity,
            client->player->inventory[PHIRAS].quantity,
            client->player->inventory[THYSTAME].quantity);
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

void send_pin_to_all(server_t *server, client_t *client)
{
    client_t *current = server->client;

    if (!server || !client || !client->player)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current != client) {
            send_pin_command(server, client, current);
        }
        current = current->next;
    }
}

void command_pin(server_t *server, client_t *client, char *buffer)
{
    client_t *recipient = NULL;
    int id = -1;

    if (!server || !client || !client)
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
