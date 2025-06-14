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
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void send_plv_to_all(server_t *server, client_t *client)
{
    client_t *current = server->client;

    if (!server || !client || !client->player)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current != client) {
            send_plv_command(server, client, current);
        }
        current = current->next;
    }
}

void command_plv(server_t *server, client_t *client, char **buffer)
{
    client_t *recipient = NULL;
    int id = -1;

    if (!server || !client || !buffer ||
        !server->graphical_clients || arr_len(buffer) != 2 ||
        sscanf(buffer[1], "#%d\n", &id) != 1)
        return write_command_output(client->client_fd, "sbp\n");
    recipient = find_client_by_id(server, id);
    if (!recipient || recipient->type != AI) {
        write_command_output(client->client_fd, "sbp\n");
        return;
    }
    send_plv_command(server, recipient, client);
}
