/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_edi
*/

#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

void send_edi_command(server_t *server, int egg_id)
{
    char *buffer = NULL;
    int size = 0;
    graphical_client_t *graphical_client = NULL;

    if (!server || !server->graphical_clients)
        return;
    buffer = NULL;
    size = snprintf(NULL, 0, "edi #%d\n", egg_id);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "edi #%d\n", egg_id);
    graphical_client = server->graphical_clients;
    while (graphical_client) {
        write_command_output(graphical_client->client->client_fd, buffer);
        graphical_client = graphical_client->next;
    }
    free(buffer);
}
