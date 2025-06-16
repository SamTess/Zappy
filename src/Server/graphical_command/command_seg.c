/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_seg
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

void command_seg(server_t *server, const char *team_name)
{
    graphical_client_t *current = NULL;
    char *buffer = NULL;
    int size = 0;

    if (!server || !server->graphical_clients || !team_name)
        return;
    size = snprintf(NULL, 0, "seg %s\n", team_name);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "seg %s\n", team_name);
    current = server->graphical_clients;
    while (current) {
        write_command_output(current->client->client_fd, buffer);
        current = current->next;
    }
    free(buffer);
}
