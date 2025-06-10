/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_init
*/

#include "../include/player.h"
#include "../include/server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static egg_t *find_egg_for_team(server_t *server, char *team_name)
{
    egg_t *current = server->eggs;

    if (team_name == NULL)
        return NULL;
    while (current != NULL) {
        if (current->team_name != NULL &&
            strcmp(current->team_name, team_name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

//! The second update of client position is a fallback maybe
//! need more error handling
void init_new_player_pos(server_t *server, client_t *new_client)
{
    int random_x = rand() % server->parsed_info->width;
    int random_y = rand() % server->parsed_info->height;
    egg_t *team_egg;

    if (new_client->player->team_name != NULL) {
        team_egg = find_egg_for_team(server, new_client->player->team_name);
        if (team_egg != NULL) {
            new_client->player->pos_x = team_egg->pos_x;
            new_client->player->pos_y = team_egg->pos_y;
            tile_add_player(&server->map[team_egg->pos_y][team_egg->pos_x],
                new_client->client_id);
            remove_egg(server, team_egg->egg_id,
                &server->map[team_egg->pos_y][team_egg->pos_x]);
            return;
        }
    }
    new_client->player->pos_x = random_x;
    new_client->player->pos_y = random_y;
    tile_add_player(&server->map[random_y][random_x], new_client->client_id);
}

static void init_pending(player_t *player)
{
    player->pending_cmd = calloc(1, sizeof(pending_cmd_t));
    player->pending_cmd->args = NULL;
    player->pending_cmd->func = NULL;
}

void init_player(player_t *player, char *playerTeam)
{
    player->pos_x = 0;
    player->pos_y = 0;
    player->rotation = UP;
    player->busy_until = 0;
    player->queue_size = 0;
    player->command_queue = calloc(10, sizeof(char *));
    if (player->command_queue == NULL)
        server_err("Malloc failed for command queue");
    player->level = 1;
    player->life = 126;
    if (playerTeam != NULL) {
        player->team_name = strdup(playerTeam);
        if (player->team_name == NULL)
            server_err("Strdup failed for player team name");
    } else
        player->team_name = NULL;
    init_inventory(player);
    player->is_in_incantation = false;
    player->incantation_leader_id = -1;
    init_pending(player);
}
