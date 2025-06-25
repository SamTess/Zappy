/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tile_to_str
*/
#include "../include/command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *repeat_word(char *word, int nb)
{
    char *res;

    if (nb <= 0)
        return strdup("");
    res = calloc(((strlen(word) * nb) + (nb + 1)), sizeof(char));
    if (!res)
        return NULL;
    res[0] = '\0';
    for (int i = 0; i < nb; ++i) {
        if (i > 0)
            strcat(res, " ");
        strcat(res, word);
    }
    return res;
}

static int calculate_total_size(int *resources, char **names)
{
    int non_empty_count = 0;
    int total_size = 2;

    for (int i = 0; i < COUNT; ++i) {
        if (resources[i] > 0) {
            total_size += (strlen(names[i]) *
                resources[i]) + (resources[i] - 1);
            non_empty_count++;
        }
    }
    if (non_empty_count > 1)
        total_size += non_empty_count - 1;
    return total_size;
}

static char *format_str_resources(char *names,
    int resources, int first_resource, char *res)
{
    char *temp = repeat_word(names, resources);

    if (!temp)
        return NULL;
    if (!first_resource)
        strcat(res, " ");
    return temp;
}

static int check_resources(char *names, int resources,
    int first_resource, char *res)
{
    char *temp;

    if (resources > 0) {
        temp = format_str_resources(names, resources,
            first_resource, res);
        if (!temp)
            return -1;
        strcat(res, temp);
        free(temp);
        first_resource = 0;
    }
    return 0;
}

static char *add_resources(int *resources)
{
    char *names[COUNT] = {"food", "linemate", "deraumere", "sibur", "mendiane",
        "phiras", "thystame"};
    size_t total_size = calculate_total_size(resources, names);
    char *res = malloc(total_size);
    int first_resource = 1;

    if (!res)
        return NULL;
    res[0] = '\0';
    for (int i = 0; i < COUNT; ++i) {
        if (check_resources(names, resources, first_resource, res) == -1)
            return NULL;
    }
    return res;
}

static int check_null(tile_t *tile, char **player, char **resources)
{
    *player = repeat_word("player", tile->player_count);
    if (!player)
        return -1;
    *resources = add_resources(tile->resources);
    if (!resources)
        return -1;
    return 0;
}

static char *check_res(int p_len, int r_len, char *player, char *resources)
{
    char *res = malloc(p_len + r_len + 2);

    if (!res)
        return NULL;
    res[0] = '\0';
    if (p_len)
        strcat(res, player);
    if (p_len && r_len)
        strcat(res, " ");
    if (r_len)
        strcat(res, resources);
    return res;
}

char *tile_to_str(tile_t *tile)
{
    char *player;
    char *resources;
    int p_len;
    int r_len;
    char *res;

    if (check_null(tile, &player, &resources) == -1)
        return NULL;
    p_len = strlen(player);
    r_len = strlen(resources);
    res = check_res(p_len, r_len, player, resources);
    free(player);
    free(resources);
    if (!res)
        return NULL;
    return res;
}
