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

void add_to_command_queue(server_t *server, client_t *client, char *command)
{
    if (!client || !client->player || !command ||
        client->player->queue_size >= 10 || !client->player->command_queue)
        return;
    client->player->command_queue[client->player->queue_size] =
        strdup(command);
    client->player->queue_size++;
    if (client->player->queue_size >= 10 && server && server->poll_manager)
        server->poll_manager->needs_rebuild = true;
}

static void check_rebuild(bool was_full, client_t *client, server_t *server)
{
    if (was_full && client->player->queue_size < 10 &&
        server && server->poll_manager)
        server->poll_manager->needs_rebuild = true;
}

void process_next_queued_command(server_t *server, client_t *client)
{
    char *next_command;
    bool was_full;

    if (!client || !client->player || !server ||
        client->player->queue_size <= 0 || !client->player->command_queue)
        return;
    was_full = (client->player->queue_size >= 10);
    next_command = client->player->command_queue[0];
    for (int i = 0; i < client->player->queue_size - 1; i++) {
        client->player->command_queue[i] =
            client->player->command_queue[i + 1];
    }
    client->player->command_queue[client->player->queue_size - 1] = NULL;
    client->player->queue_size--;
    check_rebuild(was_full, client, server);
    if (next_command) {
        execute_com(server, client, next_command);
        free(next_command);
        next_command = NULL;
    }
}
