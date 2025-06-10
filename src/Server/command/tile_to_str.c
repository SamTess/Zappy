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
    res = malloc(sizeof(char) * ((strlen(word) * nb) + (nb + 1)));
    if (!res)
        server_err("Malloc failed for res repeat_word");
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

    if (!first_resource)
        strcat(res, " ");
    return temp;
}

static char *add_resources(int *resources)
{
    char *names[COUNT] = {"food", "linemate", "deraumere", "sibur", "mendiane",
        "phiras", "thystame"};
    size_t total_size = calculate_total_size(resources, names);
    char *res = malloc(total_size);
    int first_resource = 1;
    char *temp;

    if (!res)
        server_err("Malloc failed res add_resources");
    res[0] = '\0';
    for (int i = 0; i < COUNT; ++i) {
        if (resources[i] > 0) {
            temp = format_str_resources(names[i], resources[i],
                first_resource, res);
            strcat(res, temp);
            free(temp);
            first_resource = 0;
        }
    }
    return res;
}

char *tile_to_str(tile_t *tile)
{
    char *player = repeat_word("player", tile->player_count);
    char *resources = add_resources(tile->resources);
    int p_len = strlen(player);
    int r_len = strlen(resources);
    char *res = malloc(p_len + r_len + 2);

    if (!res)
        server_err("Malloc failed res tile_to_str");
    res[0] = '\0';
    if (p_len)
        strcat(res, player);
    if (p_len && r_len)
        strcat(res, " ");
    if (r_len)
        strcat(res, resources);
    free(player);
    free(resources);
    return res;
}
