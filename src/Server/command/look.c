/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look
*/
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int get_total_size_tile(int i, char ***tiles,
    client_t *user, server_t *server)
{
    size_t total_len = 1;
    int num_tiles = 2 * i + 1;

    *tiles = malloc(num_tiles * sizeof(char *));
    if (!*tiles)
        server_err("Malloc failed for tiles array");
    for (int j = 0; j < num_tiles; j++) {
        (*tiles)[j] = check_rota_tiles(user, server, i, j);
        if (!(*tiles)[j])
            (*tiles)[j] = strdup("");
        total_len += strlen((*tiles)[j]);
        if (strlen((*tiles)[j]) > 0 && strchr((*tiles)[j], ' ') != NULL)
            total_len += 1;
        if (j > 0)
            total_len += 1;
    }
    return total_len;
}

static char *format_tile(size_t total_len, char **tiles, int num_tiles)
{
    char *res = malloc(sizeof(char) * total_len);

    if (!res)
        server_err("Malloc failed for res formatting");
    res[0] = '\0';
    for (int j = 0; j < num_tiles; j++) {
        if (j > 0)
            strcat(res, ",");
        if (strlen(tiles[j]) > 0 && strchr(tiles[j], ' ') != NULL)
            strcat(res, " ");
        strcat(res, tiles[j]);
        free(tiles[j]);
    }
    return res;
}

static char *look_tiles(client_t *user, server_t *server, int i)
{
    int num_tiles = 2 * i + 1;
    char **tiles = NULL;
    size_t total_len;
    char *res;

    total_len = get_total_size_tile(i, &tiles, user, server);
    if (!tiles)
        return strdup("");
    res = format_tile(total_len, tiles, num_tiles);
    free(tiles);
    return res;
}

static int get_total_size(char ***level_tiles,
    client_t *user, server_t *server)
{
    int total = 4;
    int num_levels = user->player->level + 1;

    *level_tiles = malloc(num_levels * sizeof(char *));
    if (!*level_tiles)
        server_err("Malloc failed for tiles allocation");
    for (int i = 0; i <= user->player->level; i++) {
        (*level_tiles)[i] = look_tiles(user, server, i);
        if (!(*level_tiles)[i])
            (*level_tiles)[i] = strdup("");
        total += strlen((*level_tiles)[i]);
        if (i > 0)
            total += 1;
    }
    return total;
}

static char *format_look(size_t total_len, char **level_tiles, client_t *user)
{
    char *res = malloc(sizeof(char) * total_len);

    if (!res)
        server_err("Malloc failed for res final look");
    strcpy(res, "[");
    for (int i = 0; i <= user->player->level; i++) {
        if (i > 0)
            strcat(res, ",");
        strcat(res, level_tiles[i]);
        free(level_tiles[i]);
    }
    strcat(res, "]\n");
    return res;
}

void look(server_t *server, client_t *user, char **buffer)
{
    char **level_tiles = NULL;
    size_t total_len = 0;
    char *res;

    if (!server || !user || !user->player || arr_len(buffer) != 1)
        return write_command_output(user->client_fd, "ko\n");
    total_len = get_total_size(&level_tiles, user, server);
    if (!level_tiles)
        return write_command_output(user->client_fd, "ko\n");
    res = format_look(total_len, level_tiles, user);
    free(level_tiles);
    write_command_output(user->client_fd, res);
    free(res);
}
