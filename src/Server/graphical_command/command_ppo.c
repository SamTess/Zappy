/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_ppo
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

client_t *find_client_by_id(server_t *server, int id)
{
    client_t *current = server->client;

    while (current != NULL) {
        if (current->client_id == id &&
            current->player && current->type == AI) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static char *get_ppo_buffer(client_t *tmp)
{
    char *buffer = NULL;
    int size = 0;

    if (!tmp || !tmp->player)
        return NULL;
    size = snprintf(NULL, 0, "ppo #%d %d %d %d\n", tmp->client_id,
        tmp->player->pos_x, tmp->player->pos_y, tmp->player->rotation);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "ppo #%d %d %d %d\n", tmp->client_id,
        tmp->player->pos_x, tmp->player->pos_y, tmp->player->rotation);
    return buffer;
}

bool send_ppo_command(server_t *server, int id)
{
    char *buffer = NULL;
    client_t *tmp = NULL;

    if (!server)
        return false;
    tmp = find_client_by_id(server, id);
    if (!tmp || !tmp->player)
        return false;
    buffer = get_ppo_buffer(tmp);
    for (client_t *cur = server->client; cur != NULL; cur = cur->next) {
        if (cur->type == GRAPHICAL && cur->is_fully_connected) {
            write_command_output(cur->client_fd, buffer);
        }
    }
    free(buffer);
    return true;
}

void command_ppo(server_t *server, client_t *client, char *buffer)
{
    int id = 0;

    if (!server || !client || !buffer || !server->graphical_clients)
        return write_command_output(client->client_fd, "ko\n");
    if (client->type != GRAPHICAL)
        return write_command_output(client->client_fd, "ko\n");
    if (sscanf(buffer, "ppo #%d\n", &id) != 1)
        return write_command_output(client->client_fd, "ko\n");
    if (id < 0)
        return write_command_output(client->client_fd, "ko\n");
    if (!send_ppo_command(server, id))
        return write_command_output(client->client_fd, "ko\n");
}
