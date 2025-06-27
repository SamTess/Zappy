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
    player_t *player, game_t *game)
{
    size_t total_len = 1;
    int num_tiles = 2 * i + 1;

    *tiles = malloc(num_tiles * sizeof(char *));
    if (!*tiles)
        return -1;
    for (int j = 0; j < num_tiles; j++) {
        (*tiles)[j] = check_rota_tiles(player, game, i, j);
        if (!(*tiles)[j])
            (*tiles)[j] = strdup("");
        if (!(*tiles)[j])
            return -1;
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
        return NULL;
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

static char *look_tiles(player_t *player, game_t *game, int i)
{
    int num_tiles = 2 * i + 1;
    char **tiles = NULL;
    size_t total_len;
    char *res;

    total_len = get_total_size_tile(i, &tiles, player, game);
    if (total_len == (size_t)-1)
        return NULL;
    if (!tiles)
        return strdup("");
    res = format_tile(total_len, tiles, num_tiles);
    if (!res)
        return NULL;
    free(tiles);
    return res;
}

static int get_total_size(char ***level_tiles,
    player_t *player, game_t *game)
{
    int total = 4;
    int num_levels = player->level + 1;

    *level_tiles = malloc(num_levels * sizeof(char *));
    if (!*level_tiles)
        return -1;
    for (int i = 0; i <= player->level; i++) {
        (*level_tiles)[i] = look_tiles(player, game, i);
        if (!(*level_tiles)[i])
            (*level_tiles)[i] = strdup("");
        if (!(*level_tiles)[i])
            return -1;
        total += strlen((*level_tiles)[i]);
        if (i > 0)
            total += 1;
    }
    return total;
}

static char *format_look(size_t total_len, char **level_tiles,
    player_t *player)
{
    char *res = malloc(sizeof(char) * total_len);

    if (!res)
        return NULL;
    strcpy(res, "[");
    for (int i = 0; i <= player->level; i++) {
        if (i > 0)
            strcat(res, ",");
        strcat(res, level_tiles[i]);
        free(level_tiles[i]);
    }
    strcat(res, "]\n");
    return res;
}

void look(game_t *game, zappy_client_t *client, zappy_client_t *clients,
    char **buffer)
{
    char **level_tiles = NULL;
    size_t total_len = 0;
    char *res;

    (void)clients;
    if (!game || !client || !client->player || arr_len(buffer) != 1)
        return write_command_output_buffer(client->client, "ko\n");
    total_len = get_total_size(&level_tiles, client->player, game);
    if (total_len == (size_t)-1 || !level_tiles)
        return write_command_output_buffer(client->client, "ko\n");
    res = format_look(total_len, level_tiles, client->player);
    if (!res)
        return write_command_output_buffer(client->client, "ko\n");
    free(level_tiles);
    write_command_output_buffer(client->client, res);
    free(res);
}
