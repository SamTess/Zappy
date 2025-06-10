/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** finish_incantation
*/

#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void handle_incantation_failure(client_t *client)
{
    client->player->incantation_leader_id = -1;
    write_command_output(client->client_fd, "ko\n");
}

static bool remove_resources(tile_t *tile, int level)
{
    int required_resources[7][COUNT] = {
        {1, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0},
        {2, 1, 1, 0, 0, 0, 0},
        {2, 2, 1, 1, 0, 0, 0},
        {4, 1, 1, 2, 1, 0, 0},
        {4, 3, 2, 1, 1, 1, 0},
        {6, 1, 2, 3, 2, 2, 1}
    };

    for (int i = FOOD; i < COUNT; i++) {
        tile->resources[i] -= required_resources[level - 1][i];
        if (tile->resources[i] < 0)
            return false;
    }
    return true;
}

static void handle_incantation_success(client_t *client,
    tile_t *tile, int old_level)
{
    char *level_str;

    client->player->level++;
    level_str = malloc(20 * sizeof(char));
    if (client->player->incantation_leader_id == client->client_id)
        remove_resources(tile, old_level + 1);
    client->player->incantation_leader_id = -1;
    sprintf(level_str, "Current level: %d\n", client->player->level);
    write_command_output(client->client_fd, level_str);
    free(level_str);
}

void finish_incantation(server_t *server, client_t *client)
{
    tile_t *tile = &server->map[client->player->pos_y][client->player->pos_x];
    int old_level;

    if (!client || !client->player || !client->player->is_in_incantation ||
        client->player->busy_until > server->current_tick)
        return;
    old_level = client->player->level;
    client->player->is_in_incantation = false;
    if (!can_start_incantation(server, client)) {
        command_pie(server, client->player->pos_x,
            client->player->pos_y, 0);
        return handle_incantation_failure(client);
    }
    handle_incantation_success(client, tile, old_level);
    command_pie(server, client->player->pos_x,
        client->player->pos_y, 1);
    send_plv_to_all(server, client);
}
