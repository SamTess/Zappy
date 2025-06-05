/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look_utils
*/
#include "../include/command.h"
#include <string.h>

static char *check_up(client_t *user, int i, int j, server_t *server)
{
    int y = (user->player->pos_y - i + server->parsed_info->height)
        % server->parsed_info->height;
    int x = (user->player->pos_x - i + j + server->parsed_info->width)
        % server->parsed_info->width;

    return tile_to_str(&server->map[y][x]);
}

static char *check_down(client_t *user, int i, int j, server_t *server)
{
    int y = (user->player->pos_y + i) % server->parsed_info->height;
    int x = (user->player->pos_x + i - j + server->parsed_info->width)
        % server->parsed_info->width;

    return tile_to_str(&server->map[y][x]);
}

static char *check_left(client_t *user, int i, int j, server_t *server)
{
    int y = (user->player->pos_y + j) % server->parsed_info->height;
    int x = (user->player->pos_x - i + server->parsed_info->width)
        % server->parsed_info->width;

    return tile_to_str(&server->map[y][x]);
}

static char *check_right(client_t *user, int i, int j, server_t *server)
{
    int y = (user->player->pos_y - j + server->parsed_info->height)
        % server->parsed_info->height;
    int x = (user->player->pos_x + i) % server->parsed_info->width;

    return tile_to_str(&server->map[y][x]);
}

char *check_rota_tiles(client_t *user, server_t *server, int i, int j)
{
    switch (user->player->rotation) {
    case UP:
        return check_up(user, i, j, server);
    case DOWN:
        return check_down(user, i, j, server);
    case LEFT:
        return check_left(user, i, j, server);
    case RIGHT:
        return check_right(user, i, j, server);
    default:
        return strdup("");
    }
}
