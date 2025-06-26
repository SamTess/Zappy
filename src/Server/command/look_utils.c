/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look_utils
*/
#include "../include/command.h"
#include <string.h>

static char *check_up(player_t *player, int i, int j, game_t *game)
{
    int y = (player->pos_y - i + game->parsed_info->height)
        % game->parsed_info->height;
    int x = (player->pos_x - i + j + game->parsed_info->width)
        % game->parsed_info->width;
    char *res = tile_to_str(&game->map[y][x]);

    if (!res)
        return NULL;
    return res;
}

static char *check_down(player_t *player, int i, int j, game_t *game)
{
    int y = (player->pos_y + i) % game->parsed_info->height;
    int x = (player->pos_x + i - j + game->parsed_info->width)
        % game->parsed_info->width;
    char *res = tile_to_str(&game->map[y][x]);

    if (!res)
        return NULL;
    return res;
}

static char *check_left(player_t *player, int i, int j, game_t *game)
{
    int y = (player->pos_y + j) % game->parsed_info->height;
    int x = (player->pos_x - i + game->parsed_info->width)
        % game->parsed_info->width;
    char *res = tile_to_str(&game->map[y][x]);

    if (!res)
        return NULL;
    return res;
}

static char *check_right(player_t *player, int i, int j, game_t *game)
{
    int y = (player->pos_y - j + game->parsed_info->height)
        % game->parsed_info->height;
    int x = (player->pos_x + i) % game->parsed_info->width;
    char *res = tile_to_str(&game->map[y][x]);

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

char *check_rota_tiles(player_t *player, game_t *game, int i, int j)
{
    char *res;

    switch (player->rotation) {
    case UP:
        res = check_up(player, i, j, game);
        break;
    case DOWN:
        res = check_down(player, i, j, game);
        break;
    case LEFT:
        res = check_left(player, i, j, game);
        break;
    case RIGHT:
        res = check_right(player, i, j, game);
        break;
    default:
        res = strdup("");
    }
    return check_res(res);
}
