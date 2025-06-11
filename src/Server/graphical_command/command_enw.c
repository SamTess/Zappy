/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_enw
*/

#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

static char *get_buffer_for_enw(int egg_id, int client_id,
    int pos_x, int pos_y)
{
    char *buffer = NULL;
    int size = snprintf(NULL, 0, "enw #%d #%d %d %d\n",
            egg_id, client_id, pos_x, pos_y);

    buffer = malloc(size + 1);
    if (buffer) {
        snprintf(buffer, size + 1, "enw #%d #%d %d %d\n",
                egg_id, client_id, pos_x, pos_y);
    }
    return buffer;
}

void send_enw_command(server_t *server, client_t *client, int egg_id)
{
    char *buffer = NULL;
    graphical_client_t *graphical_client = NULL;

    if (!server || !client || !client->player || !server->graphical_clients)
        return;
    buffer = get_buffer_for_enw(egg_id, client->client_id,
        client->player->pos_x, client->player->pos_y);
    if (!buffer)
        return;
    graphical_client = server->graphical_clients;
    while (graphical_client) {
        write_command_output(graphical_client->client->client_fd, buffer);
        graphical_client = graphical_client->next;
    }
    free(buffer);
}
