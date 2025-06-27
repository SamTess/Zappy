/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_ppo
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

zappy_client_t *find_client_by_id(zappy_client_t *clients, int id)
{
    zappy_client_t *current = clients;

    while (current != NULL) {
        if (current->client && current->client->client_id == id &&
            current->player && current->type == AI) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static int calculate_ppo_buffer_size(zappy_client_t *client)
{
    if (!client || !client->player || !client->client)
        return 0;
    return snprintf(NULL, 0, "ppo #%d %d %d %d\n", client->client->client_id,
        client->player->pos_x, client->player->pos_y,
        client->player->rotation);
}

static char *format_ppo_response(zappy_client_t *client)
{
    int size;
    char *buffer;

    if (!client || !client->player || !client->client)
        return NULL;
    size = calculate_ppo_buffer_size(client);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "ppo #%d %d %d %d\n", client->client->client_id,
        client->player->pos_x, client->player->pos_y,
        client->player->rotation);
    return buffer;
}

static void send_command(zappy_client_t *clients, char *buffer)
{
    zappy_client_t *current = clients;

    while (current) {
        if (current->type == GRAPHICAL && current->client &&
            current->is_fully_connected && current->client->client_id != -1)
            write_command_output_buffer(current->client, buffer);
        current = current->next;
    }
}

bool send_ppo_command(game_t *game, zappy_client_t *clients, int id)
{
    char *buffer = NULL;
    zappy_client_t *target = NULL;

    if (!game || !clients)
        return false;
    target = find_client_by_id(clients, id);
    if (!target || !target->player)
        return false;
    buffer = format_ppo_response(target);
    if (!buffer)
        return false;
    send_command(clients, buffer);
    free(buffer);
    return true;
}

void command_ppo(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer)
{
    int id = 0;

    if (!game || !client || !client->client || !buffer || !clients ||
        client->type != GRAPHICAL || arr_len(buffer) != 2 ||
        sscanf(buffer[1], "#%d", &id) != 1 || id < 0 ||
        !find_client_by_id(clients, id) ||
        !send_ppo_command(game, clients, id)){
        return write_command_output_buffer(client->client, "sbp\n");
    }
}
