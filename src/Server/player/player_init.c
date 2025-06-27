/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_init
*/

#include "../include/player.h"
#include "../include/game.h"
#include "../include/zappy.h"
#include "../include/graphical_commands.h"
#include "../include/egg.h"
#include "../include/tile.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static egg_t *find_egg_for_team(game_t *game, char *team_name)
{
    egg_t *current = game->eggs;

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

static int add_player_egg_team(game_t *game,
    zappy_client_t *new_client, zappy_client_t *clients)
{
    egg_t *team_egg = find_egg_for_team(game, new_client->player->team_name);

    if (team_egg != NULL) {
        new_client->player->pos_x = team_egg->pos_x;
        new_client->player->pos_y = team_egg->pos_y;
        tile_add_player(&game->map[team_egg->pos_y][team_egg->pos_x],
            new_client->client->client_id);
        send_ebo_command(game, clients, team_egg->egg_id);
        remove_egg(game, team_egg->egg_id,
            &game->map[team_egg->pos_y][team_egg->pos_x]);
        return 0;
    }
    return -1;
}

//! The second update of client position is a fallback maybe
//! need more error handling
void init_new_player_pos(game_t *game, zappy_client_t *new_client,
    zappy_client_t *clients)
{
    int random_x = rand() % game->parsed_info->width;
    int random_y = rand() % game->parsed_info->height;

    if (new_client->player->team_name != NULL) {
        if (add_player_egg_team(game, new_client, clients) == 0)
            return;
    }
    new_client->player->pos_x = random_x;
    new_client->player->pos_y = random_y;
    tile_add_player(&game->map[random_y][random_x],
        new_client->client->client_id);
}

static void init_pending(player_t *player)
{
    player->pending_cmd = calloc(1, sizeof(pending_cmd_t));
    if (!player->pending_cmd)
        return;
    player->pending_cmd->args = NULL;
    player->pending_cmd->func = NULL;
}

static int init_player_team(player_t *player, char *player_team)
{
    player->team_name = strdup(player_team);
    if (player->team_name == NULL){
        free(player->command_queue);
        return -1;
    }
    return 0;
}

static int check_command_queue(player_t *player)
{
    player->command_queue = calloc(10, sizeof(char *));
    if (player->command_queue == NULL){
        free(player);
        player = NULL;
        return -1;
    }
    init_pending(player);
    if (!player->pending_cmd){
        free(player->command_queue);
        free(player);
        player = NULL;
        return -1;
    }
    return 0;
}

void init_player(player_t *player, char *player_team)
{
    player->pos_x = 0;
    player->pos_y = 0;
    player->rotation = rand() % 4 + 1;
    player->busy_until = 0;
    player->queue_size = 0;
    if (check_command_queue(player) == -1)
        return;
    player->level = 1;
    player->life = 126;
    if (player_team != NULL) {
        if (init_player_team(player, player_team) == -1)
            player->team_name = NULL;
    } else
        player->team_name = NULL;
    init_inventory(player);
    player->is_in_incantation = false;
    player->incantation_leader_id = -1;
}
