/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_sst
*/

#include "../include/command.h"
#include <stdlib.h>
#include <stdio.h>

static char *get_buffer_sst(int time)
{
    char *buffer = NULL;
    int size = snprintf(NULL, 0, "sst %d\n", time);

    buffer = malloc(size + 1);
    if (!buffer) {
        return NULL;
    }
    snprintf(buffer, size + 1, "sst %d\n", time);
    return buffer;
}

static int get_time_from_buffer(char *buffer)
{
    int time = 0;

    if (sscanf(buffer, "sst %d\n", &time) != 1 || time < 0) {
        return -1;
    }
    return time;
}

void command_sst(server_t *server, client_t *client, char *buffer)
{
    int time;
    graphical_client_t *graphical_client = NULL;
    char *tmp_buffer = NULL;

    if (!buffer || client->type != GRAPHICAL)
        return;
    time = get_time_from_buffer(buffer);
    if (time < 0)
        return;
    tmp_buffer = get_buffer_sst(time);
    server->parsed_info->frequence = time;
    graphical_client = server->graphical_clients;
    while (graphical_client) {
        write_command_output(graphical_client->client->client_fd,
            tmp_buffer);
        graphical_client = graphical_client->next;
    }
    free(tmp_buffer);
}
