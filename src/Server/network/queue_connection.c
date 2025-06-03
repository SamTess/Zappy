/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** queue_conection
*/

#include "../include/server.h"
#include "../include/command.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void add_to_command_queue(client_t *client, char *command)
{
    if (!client || !client->player || !command)
        return;
    if (client->player->queue_size >= 10)
        return;
    if (!client->player->command_queue)
        return;
    client->player->command_queue[client->player->queue_size] =
        strdup(command);
    client->player->queue_size++;
}

void process_next_queued_command(server_t *server, client_t *client)
{
    char *next_command;

    if (!client || !client->player || !server)
        return;
    if (client->player->queue_size <= 0 || !client->player->command_queue)
        return;
    next_command = client->player->command_queue[0];
    for (int i = 0; i < client->player->queue_size - 1; i++) {
        client->player->command_queue[i] =
            client->player->command_queue[i + 1];
    }
    client->player->command_queue[client->player->queue_size - 1] = NULL;
    client->player->queue_size--;
    if (next_command) {
        execute_com(server, client, next_command);
        free(next_command);
        next_command = NULL;
    }
}
