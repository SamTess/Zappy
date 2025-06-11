/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_ebo
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/graphical_commands.h"
#include <stdio.h>

void send_ebo_command(server_t *server, int egg_id)
{
    graphical_client_t *current = NULL;
    char buffer[32];
    if (!server && !server->graphical_clients)
        return;
    snprintf(buffer, sizeof(buffer), "ebo #%d\n", egg_id);
    current = server->graphical_clients;
    while (current) {
        write_command_output(current->client->client_fd, buffer);
        current = current->next;
    }
}
