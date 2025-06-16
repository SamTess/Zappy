/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_tna
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

static void send_one_tna_command(server_t *server, client_t *client,
    const char *team)
{
    int size = 0;
    char *buffer = NULL;

    if (!server || !client || !team)
        return;
    size = snprintf(NULL, 0, "tna %s\n", team);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "tna %s\n", team);
    write_command_output(client->client_fd, buffer);
    free(buffer);
}

void send_tna_command(server_t *server, client_t *client)
{
    if (!server || !client)
        return;
    for (int i = 0; server->parsed_info->names[i]; i++) {
        send_one_tna_command(server, client, server->parsed_info->names[i]);
    }
}

void command_tna(server_t *server, client_t *client, char **buffer)
{
    if (!server || !client || !buffer || !server->parsed_info ||
        !server->parsed_info->names || !server->graphical_clients
        || arr_len(buffer) != 1)
        return write_command_output(client->client_fd, "sbp\n");
    send_tna_command(server, client);
}
