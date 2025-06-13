/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_smg
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void send_smg_command(server_t *server, const char *msg)
{
    graphical_client_t *current = NULL;
    char *buffer = NULL;
    int size = 0;

    if (!server || !server->graphical_clients || !msg)
        return;
    size = snprintf(NULL, 0, "smg %s\n", msg);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "smg %s\n", msg);
    current = server->graphical_clients;
    while (current) {
        write_command_output(current->client->client_fd, buffer);
        current = current->next;
    }
    free(buffer);
}
