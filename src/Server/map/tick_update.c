/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tick_update
*/

#include "../include/server.h"
#include "../include/command.h"
#include "../include/player.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static bool is_client_valid(server_t *server, client_t *target)
{
    client_t *current = server->client;

    while (current != NULL) {
        if (current == target)
            return true;
        current = current->next;
    }
    return false;
}

static void tick_check(server_t *server, client_t *current)
{
    if (current != NULL && current->player != NULL)
        check_player_starvation(server, current);
    if (is_client_valid(server, current) && current != NULL &&
        current->player != NULL)
        finish_incantation(server, current);
}

void update_game_tick(server_t *server)
{
    client_t *current = server->client;
    client_t *next;

    server->current_tick++;
    if (current != NULL)
        current = current->next;
    while (current != NULL) {
        next = current->next;
        tick_check(server, current);
        if (is_client_valid(server, current) && current->player &&
            current->player->busy_until <= server->current_tick &&
            current->player->queue_size > 0)
            process_next_queued_command(server, current);
        current = next;
    }
}
