/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pie
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

static char *get_pie_buffer(int x, int y, int result)
{
    char *buffer = NULL;
    int size = 0;

    size = snprintf(NULL, 0, "pie %d %d %d\n", x, y, result);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pie %d %d %d\n", x, y, result);
    return buffer;
}

void command_pie(server_t *server, int x, int y, int result)
{
    client_t *current = NULL;
    char *buffer = NULL;

    if (!server || !server->graphical_clients)
        return;
    buffer = get_pie_buffer(x, y, result);
    if (!buffer)
        return;
    current = server->client;
    while (current) {
        write_command_output(current->client_fd, buffer);
        current = current->next;
    }
    free(buffer);
}
