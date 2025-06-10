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

void send_sgt_command(server_t *server, client_t *client)
{
    char *buffer = NULL;
    int size = 0;

    if (!server || !client)
        return;
    buffer = NULL;
    size = snprintf(NULL, 0, "sgt %d\n", server->parsed_info->frequence);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "sgt %d\n", server->parsed_info->frequence);
    write_command_output(client->client_fd, buffer);
    free(buffer);
}
