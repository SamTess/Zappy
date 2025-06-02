/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look
*/
#include "../include/command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

static char *check_rota_tiles(client_t *user, server_t *server, int i, int j)
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

static void check_formatting(char **tile, char **res)
{
    if (strlen(*tile) > 0 && strchr(*tile, ' ') != NULL) {
        *res = realloc(*res, strlen(*res) + 3);
        strcat(*res, ", ");
    } else {
        *res = realloc(*res, strlen(*res) + 2);
        strcat(*res, ",");
    }
}

static char *look_tiles(client_t *user, server_t *server, int i)
{
    char *res = strdup("");
    char *tile;

    for (int j = 0; j < 2 * i + 1; ++j) {
        tile = check_rota_tiles(user, server, i, j);
        if (j > 0)
            check_formatting(&tile, &res);
        res = realloc(res, strlen(res) + strlen(tile) + 1);
        strcat(res, tile);
        free(tile);
    }
    return res;
}

void look(server_t *server, client_t *user, char *buffer)
{
    char *res = strdup("[");
    char *tiles;

    (void)buffer;
    for (int i = 0; i <= user->player->level; ++i) {
        tiles = look_tiles(user, server, i);
        if (i > 0) {
            res = realloc(res, strlen(res) + 2);
            strcat(res, ",");
        }
        res = realloc(res, strlen(res) + strlen(tiles) + 1);
        strcat(res, tiles);
        free(tiles);
    }
    res = realloc(res, strlen(res) + 3);
    strcat(res, "]\n");
    write_command_output(user->client_fd, res);
    free(res);
}
