/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pbc
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

void command_pbc(server_t *server, client_t *client, char *buffer)
{
    graphical_client_t *graphical_client = NULL;
    char *msg = NULL;
    int size = 0;

    if (!server || !client || !buffer ||
        !server->graphical_clients || !client->player)
        return;
    size = snprintf(NULL, 0, "pbc #%d %s", client->client_id, buffer);
    msg = malloc(size + 1);
    if (!msg)
        return;
    snprintf(msg, size + 1, "pbc #%d %s", client->client_id, buffer);
    graphical_client = server->graphical_clients;
    while (graphical_client) {
        write_command_output(graphical_client->client->client_fd, msg);
        graphical_client = graphical_client->next;
    }
    free(msg);
}
