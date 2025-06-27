/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** right
*/
#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/parsing.h"
#include <stdio.h>

static void change_rot(player_t *player)
{
    switch (player->rotation) {
    case RIGHT:
        player->rotation = DOWN;
        break;
    case DOWN:
        player->rotation = LEFT;
        break;
    case LEFT:
        player->rotation = UP;
        break;
    case UP:
        player->rotation = RIGHT;
        break;
    default:
        break;
    }
}

void right(game_t *game, zappy_client_t *client, zappy_client_t *clients,
    char **buffer)
{
    if (!game || !client || !client->client || !client->player ||
        !clients || arr_len(buffer) != 1)
        return write_command_output_buffer(client->client, "ko\n");
    if (client->player->rotation != RIGHT && client->player->rotation != DOWN
        && client->player->rotation != LEFT
        && client->player->rotation != UP) {
        perror("Unexpected right rotation");
        return write_command_output_buffer(client->client, "ko\n");
    }
    change_rot(client->player);
    send_ppo_command(game, clients, client->client->client_id);
    write_command_output_buffer(client->client, "ok\n");
}
