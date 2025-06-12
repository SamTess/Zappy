/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** start_incantation
*/

#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/parsing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static client_t *get_client_by_id_player(server_t *server, int player_id)
{
    client_t *tmp = server->client->next;

    while (tmp != NULL) {
        if (tmp->player != NULL && tmp->client_id == player_id)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

static int nb_valid_players_on_tile(server_t *server, tile_t *tile,
    int current_level)
{
    int count = 0;

    for (int i = 0; i < tile->player_count; i++) {
        if (tile->player_ids[i] != -1 &&
            get_client_by_id_player(server, tile->player_ids[i])->
            player->level == current_level) {
            count++;
        }
    }
    return count;
}

static void set_busy_until(server_t *server, client_t *client, int duration)
{
    client->player->busy_until = server->current_tick + duration;
    client->player->is_in_incantation = true;
}

static void set_busy_all(server_t *server, client_t *client, int duration)
{
    tile_t *tile = &server->map[client->player->pos_y][client->player->pos_x];
    client_t *tmp_client;

    for (int i = 0; i < tile->player_count; i++) {
        tmp_client = get_client_by_id_player(server, tile->player_ids[i]);
        if (tmp_client && tmp_client->player &&
            tmp_client->player->level == client->player->level) {
            tmp_client->player->incantation_leader_id = client->client_id;
            set_busy_until(server, tmp_client, duration);
        }
    }
}

static bool has_enough_resources(tile_t *tile, int level)
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
        if (tile->resources[i] < required_resources[level - 1][i])
            return false;
    }
    return true;
}

static int how_many_players_needed(int level)
{
    int required_players[7] = {1, 2, 2, 4, 4, 6, 6};

    if (level < 1 || level > 7)
        return -1;
    return required_players[level - 1];
}

bool can_start_incantation(server_t *server, client_t *client)
{
    int required_players = 0;
    int prerequisite_level = client->player->level + 1;
    tile_t *tile = &server->map[client->player->pos_y][client->player->pos_x];
    int current_players = 0;

    if (prerequisite_level > 7 ||
        !has_enough_resources(tile, prerequisite_level))
        return false;
    required_players = how_many_players_needed(prerequisite_level);
    current_players =
        nb_valid_players_on_tile(server, tile, client->player->level);
    if (current_players < required_players)
        return false;
    return true;
}

void start_incantation(server_t *server, client_t *client, char **buffer)
{
    if (!server || !client || !client->player || arr_len(buffer) != 1)
        return write_command_output(client->client_fd, "ko\n");
    if (!can_start_incantation(server, client))
        return write_command_output(client->client_fd, "ko\n");
    command_pic(server, client->player->pos_x, client->player->pos_y,
        client->player->level);
    set_busy_all(server, client, 300);
    client->player->is_in_incantation = true;
    client->player->incantation_leader_id = client->client_id;
    write_command_output(client->client_fd, "Elevation underway\n");
}
