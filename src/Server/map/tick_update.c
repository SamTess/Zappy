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

static bool tick_check(server_t *server, client_t *current)
{
    bool dead = false;

    if (current != NULL && current->player != NULL)
        dead = check_player_starvation(server, current);
    if (dead == false && current != NULL &&
        current->player != NULL && current->player->is_in_incantation)
        finish_incantation(server, current);
    return dead;
}

void update_game_tick(server_t *server)
{
    client_t *current = server->client;
    client_t *next;

    server->current_tick++;
    if (server->current_tick % 20 == 0)
        respawn_resources(server->map, server, server->total_resources,
            server->current_resources);
    if (current != NULL)
        current = current->next;
    while (current != NULL) {
        next = current->next;
        if (current->player && current->player->pending_cmd &&
            current->player->busy_until <= server->current_tick)
            execute_pending_cmd(server, current);
        if (tick_check(server, current) == false && current->player &&
            current->player->busy_until <= server->current_tick &&
            current->player->queue_size > 0)
            process_next_queued_command(server, current);
        current = next;
    }
}
