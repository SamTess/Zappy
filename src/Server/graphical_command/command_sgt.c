/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_sgt
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

void command_sgt(server_t *server, client_t *client, char *buffer)
{
    char *tmp = NULL;
    int size = 0;

    (void)buffer;
    if (!server || !client || !server->graphical_clients)
        return;
    tmp = NULL;
    size = snprintf(NULL, 0, "sgt %d\n", server->parsed_info->frequence);
    tmp = malloc(size + 1);
    if (!tmp)
        return;
    snprintf(tmp, size + 1, "sgt %d\n", server->parsed_info->frequence);
    write_command_output(client->client_fd, tmp);
    free(tmp);
}
