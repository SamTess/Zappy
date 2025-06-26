/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** forward
*/
#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <unistd.h>

static void wrap_position(game_t *game, player_t *player)
{
    if (player->pos_y >= game->parsed_info->height)
        player->pos_y = 0;
    if (player->pos_y < 0)
        player->pos_y = game->parsed_info->height - 1;
    if (player->pos_x < 0)
        player->pos_x = game->parsed_info->width - 1;
    if (player->pos_x >= game->parsed_info->width)
        player->pos_x = 0;
}

static void change_map_pos(game_t *game, zappy_client_t *client)
{
    tile_remove_player(
        &game->map[client->player->pos_y][client->player->pos_x],
        client->client->client_id);
    if (client->player->rotation == UP)
        client->player->pos_y--;
    if (client->player->rotation == DOWN)
        client->player->pos_y++;
    if (client->player->rotation == LEFT)
        client->player->pos_x--;
    if (client->player->rotation == RIGHT)
        client->player->pos_x++;
    wrap_position(game, client->player);
    tile_add_player(
        &game->map[client->player->pos_y][client->player->pos_x],
        client->client->client_id);
}

void forward(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    (void)clients;
    if (!game || !client || !client->client || !client->player ||
        arr_len(buffer) != 1) {
        write_command_output_buffer(client->client, "ko\n");
        return;
    }
    if (client->player->rotation != UP && client->player->rotation != DOWN
        && client->player->rotation != LEFT
        && client->player->rotation != RIGHT) {
        perror("Unexpected forward rotation");
        write_command_output_buffer(client->client, "ko\n");
        return;
    }
    change_map_pos(game, client);
    send_ppo_command(game, clients, client->client->client_id);
    write_command_output_buffer(client->client, "ok\n");
}
