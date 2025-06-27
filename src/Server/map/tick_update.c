/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tick_update
*/

#include "../include/server.h"
#include "../include/command.h"
#include "../include/player.h"
#include "../include/pending_cmd_utils.h"
#include "../include/tile.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static bool tick_check(game_t *game, server_t *server,
    zappy_client_t *current, zappy_client_t *clients)
{
    bool dead = false;

    if (current != NULL && current->player != NULL)
        dead = check_player_starvation(game, server, current, clients);
    if (dead == false && current != NULL &&
        current->player != NULL && current->player->is_in_incantation)
        finish_incantation(game, current, clients);
    return dead;
}

void update_game_tick(game_t *game, server_t *server, zappy_client_t *clients)
{
    zappy_client_t *current = clients;
    zappy_client_t *next;

    game->current_tick++;
    if (game->current_tick % 20 == 0)
        respawn_resources(game, clients);
    if (current != NULL)
        current = current->next;
    while (current != NULL) {
        next = current->next;
        if (current->player && current->player->pending_cmd &&
            current->player->busy_until <= game->current_tick)
            execute_pending_cmd(game, current, clients);
        if (tick_check(game, server, current, clients) == false
            && current->player &&
            current->player->busy_until <= game->current_tick &&
            current->player->queue_size > 0)
            process_next_queued_command(game, server, current, clients);
        current = next;
    }
}
