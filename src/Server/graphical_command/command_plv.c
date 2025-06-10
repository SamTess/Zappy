/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_plv
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

void send_plv_command(server_t *server, client_t *client, client_t *recipient)
{
    int size = 0;
    char *buffer = NULL;

    if (!server || !client || !client->player)
        return;
    size = snprintf(NULL, 0, "plv #%d %d\n",
                client->client_id,
                client->player->level);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "plv #%d %d\n",
            client->client_id,
            client->player->level);
    write_command_output(recipient->client_fd, buffer);
    free(buffer);
}
