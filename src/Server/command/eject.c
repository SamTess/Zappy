/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** eject
*/

#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void convert_rotation_to_vector(player_t *player, float *x, float *y)
{
    switch (player->rotation) {
        case RIGHT:
            *x = 1.0;
            *y = 0.0;
            break;
        case DOWN:
            *x = 0.0;
            *y = 1.0;
            break;
        case LEFT:
            *x = -1.0;
            *y = 0.0;
            break;
        case UP:
        default:
            *x = 0.0;
            *y = -1.0;
            break;
    }
}

static void wrap_position(game_t *game, int *x, int *y)
{
    if (*y >= game->parsed_info->height)
        *y = 0;
    if (*y < 0)
        *y = game->parsed_info->height - 1;
    if (*x < 0)
        *x = game->parsed_info->width - 1;
    if (*x >= game->parsed_info->width)
        *x = 0;
}

static int direction_push(float x, float y)
{
    if (x == 1 && y == 0)
        return 1;
    if (x == 0 && y == 1)
        return 7;
    if (x == 0 && y == -1)
        return 3;
    if (x == -1 && y == 0)
        return 5;
    return -1;
}

static char *get_string_to_send(float x, float y)
{
    char *string_to_send = malloc(sizeof(char) * 10);

    if (!string_to_send)
        return NULL;
    sprintf(string_to_send, "eject: %d\n", direction_push(x, y));
    return string_to_send;
}

static void push_single_client(game_t *game, zappy_client_t *tmp,
    char *msg, float *direction, zappy_client_t *clients)
{
    int old_x = tmp->player->pos_x;
    int old_y = tmp->player->pos_y;
    int new_x = tmp->player->pos_x + (int)direction[0];
    int new_y = tmp->player->pos_y + (int)direction[1];

    wrap_position(game, &new_x, &new_y);
    tile_remove_player(&game->map[old_y][old_x], tmp->client->client_id);
    tmp->player->pos_x = new_x;
    tmp->player->pos_y = new_y;
    tile_add_player(&game->map[new_y][new_x], tmp->client->client_id);
    send_ppo_command(game, clients, tmp->client->client_id);
    write_command_output_buffer(tmp->client, msg);
}

int push_client(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, float x, float y)
{
    zappy_client_t *tmp = clients;
    int old_x = sender->player->pos_x;
    int old_y = sender->player->pos_y;
    char *msg = get_string_to_send(x, y);

    if (msg == NULL)
        return 84;
    while (tmp) {
        if (tmp == sender || tmp->type == GRAPHICAL || !tmp->player) {
            tmp = tmp->next;
            continue;
        }
        if (tmp->player->pos_x == old_x && tmp->player->pos_y == old_y)
            push_single_client(game, tmp, msg, (float[]){x, y}, clients);
        tmp = tmp->next;
    }
    free(msg);
    return 0;
}

static void push_eggs(game_t *game, int old_x, int old_y, zappy_client_t *clients)
{
    egg_t *tmp_egg = game->eggs;
    egg_t *next_egg;

    tmp_egg = game->eggs;
    while (tmp_egg) {
        next_egg = tmp_egg->next;
        if (tmp_egg->pos_x == old_x && tmp_egg->pos_y == old_y) {
            send_edi_command(game, clients, tmp_egg->egg_id);
            remove_egg(game, tmp_egg->egg_id, &game->map[old_y][old_x]);
        }
        tmp_egg = next_egg;
    }
}

void eject(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    float x = 0;
    float y = 0;

    if (!game || !client || !client->client || !client->player ||
        !clients || arr_len(buffer) != 1)
        return write_command_output_buffer(client->client, "ko\n");
    convert_rotation_to_vector(client->player, &x, &y);
    if (push_client(game, client, clients, x, y) == 84)
        return write_command_output_buffer(client->client, "ko\n");
    push_eggs(game, client->player->pos_x, client->player->pos_y, clients);
    command_pex(game, client, clients);
    write_command_output_buffer(client->client, "ok\n");
}
