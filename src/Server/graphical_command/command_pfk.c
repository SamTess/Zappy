/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pfk
*/

#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

void command_pfk(server_t *server, client_t *client)
{
    graphical_client_t *graphical_client = NULL;
    char *buffer = NULL;
    int size = 0;

    if (client == NULL || client->player == NULL)
        return write_command_output(client->client_fd, "ko\n");
    size = snprintf(NULL, 0, "pfk #%d\n", client->client_id);
    buffer = malloc(size + 1);
    if (buffer == NULL)
        return write_command_output(client->client_fd, "ko\n");
    sprintf(buffer, "pfk #%d\n", client->client_id);
    graphical_client = server->graphical_clients;
    while (graphical_client != NULL) {
        write_command_output(graphical_client->client->client_fd, buffer);
        graphical_client = graphical_client->next;
    }
    free(buffer);
}
