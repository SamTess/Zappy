/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** finish_incantation
*/

#include "../include/command.h"
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

//? This is the function that needs to send the winner team's name
static void handle_win(char *winner, server_t *server)
{
    server->should_run = false;
    printf("%s team are the winner of this Zappy tournament\n", winner);
}

static void check_win(client_t *client, server_t *server)
{
    int count = 0;
    char *team_name = client->player->team_name;
    client_t *temp = server->client;

    while (temp){
        if (temp->player && temp->player->level == 8 &&
            strcmp(temp->player->team_name, team_name) == 0)
            count++;
    }
    if (count >= 6)
        handle_win(team_name, server);
}

static void handle_incantation_success(client_t *client,
    tile_t *tile, int old_level, server_t *server)
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
    if (client->player->level == 8)
        check_win(client, server);
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
    if (!can_start_incantation(server, client))
        return handle_incantation_failure(client);
    handle_incantation_success(client, tile, old_level, server);
}
