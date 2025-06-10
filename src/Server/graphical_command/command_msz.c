/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_msz
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

void send_msz_command(server_t *server, client_t *client)
{
    char *buffer = NULL;
    int size = 0;

    if (!server || !client)
        return;
    buffer = NULL;
    size = snprintf(NULL, 0, "msz %d %d\n",
            server->parsed_info->width, server->parsed_info->height);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "msz %d %d\n",
            server->parsed_info->width, server->parsed_info->height);
    write_command_output(client->client_fd, buffer);
    free(buffer);
}

void command_msz(server_t *server, client_t *client, char *buffer)
{
    if (!server || !client || !buffer)
        return;
    if (client->type != GRAPHICAL) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    send_msz_command(server, client);
}
