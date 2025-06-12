/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pgt
*/

#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

void command_pgt(server_t *server, client_t *client,
    resource_type_t resource_type)
{
    graphical_client_t *graphical_client = NULL;
    char *buffer = NULL;
    int size = 0;

    if (!client || !client->player || !server->graphical_clients)
        return;
    size = snprintf(NULL, 0, "pgt #%d %d\n", client->client_id, resource_type);
    buffer = malloc(size + 1);
    if (buffer == NULL)
        return;
    sprintf(buffer, "pgt #%d %d\n", client->client_id, resource_type);
    graphical_client = server->graphical_clients;
    while (graphical_client != NULL) {
        write_command_output(graphical_client->client->client_fd, buffer);
        graphical_client = graphical_client->next;
    }
    free(buffer);
}
