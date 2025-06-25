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
    char *res = tile_to_str(&server->map[y][x]);

    if (!res)
        return NULL;
    return res;
}

static char *check_down(client_t *user, int i, int j, server_t *server)
{
    int y = (user->player->pos_y + i) % server->parsed_info->height;
    int x = (user->player->pos_x + i - j + server->parsed_info->width)
        % server->parsed_info->width;
    char *res = tile_to_str(&server->map[y][x]);

    if (!res)
        return NULL;
    return res;
}

static char *check_left(client_t *user, int i, int j, server_t *server)
{
    int y = (user->player->pos_y + j) % server->parsed_info->height;
    int x = (user->player->pos_x - i + server->parsed_info->width)
        % server->parsed_info->width;
    char *res = tile_to_str(&server->map[y][x]);

    if (!res)
        return NULL;
    return res;
}

static char *check_right(client_t *user, int i, int j, server_t *server)
{
    int y = (user->player->pos_y - j + server->parsed_info->height)
        % server->parsed_info->height;
    int x = (user->player->pos_x + i) % server->parsed_info->width;
    char *res = tile_to_str(&server->map[y][x]);

    if (!res)
        return NULL;
    return res;
}

static char *check_res(char *res)
{
    if (!res)
        return NULL;
    return res;
}

char *check_rota_tiles(client_t *user, server_t *server, int i, int j)
{
    char *res;

    switch (user->player->rotation) {
    case UP:
        res = check_up(user, i, j, server);
        break;
    case DOWN:
        res = check_down(user, i, j, server);
        break;
    case LEFT:
        res = check_left(user, i, j, server);
        break;
    case RIGHT:
        res = check_right(user, i, j, server);
        break;
    default:
        res = strdup("");
    }
    return check_res(res);
}
