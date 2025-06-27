/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pnw
*/

#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>

static int calculate_size_pnw_command(player_t *player, int client_id)
{
    if (!player)
        return 0;
    return snprintf(NULL, 0, "pnw #%d %d %d %d %d %s\n",
        client_id, player->pos_x, player->pos_y, player->rotation,
        player->level, player->team_name);
}

static char *format_pnw_response(player_t *player, int client_id)
{
    int size;
    char *buffer;

    if (!player)
        return NULL;
    size = calculate_size_pnw_command(player, client_id);
    buffer = malloc(size + 1);
    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "pnw #%d %d %d %d %d %s\n",
        client_id, player->pos_x, player->pos_y, player->rotation,
        player->level, player->team_name);
    return buffer;
}

void send_pnw_command(game_t *game, zappy_client_t *clients,
    zappy_client_t *sender, zappy_client_t *recipient)
{
    char *buffer;

    (void)game;
    (void)clients;
    if (!sender || !sender->player || !recipient || !recipient->client)
        return;
    buffer = format_pnw_response(sender->player, sender->client->client_id);
    if (buffer) {
        write_command_output_buffer(recipient->client, buffer);
        free(buffer);
    }
}

void send_pnw_command_to_all(game_t *game, zappy_client_t *clients,
    zappy_client_t *sender)
{
    zappy_client_t *current = clients;

    (void)game;
    if (!clients || !sender || !sender->player)
        return;
    while (current) {
        if (current->type == GRAPHICAL && current->client &&
            current->is_fully_connected && current->client->client_id != -1)
            send_pnw_command(game, clients, sender, current);
        current = current->next;
    }
}

void send_all_player_info_to_one_client(game_t *game, zappy_client_t *clients,
    zappy_client_t *recipient)
{
    zappy_client_t *current = clients;

    (void)game;
    if (!clients || !recipient || !recipient->client
        || recipient->type != GRAPHICAL)
        return;
    while (current) {
        if (current->player && current->type == AI &&
            current->is_fully_connected)
            send_pnw_command(game, clients, current, recipient);
        current = current->next;
    }
}
