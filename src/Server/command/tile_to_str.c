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
    res[0] = '\0';
    for (int i = 0; i < nb; ++i) {
        if (i > 0)
            strcat(res, " ");
        strcat(res, word);
    }
    return res;
}

static char *add_resources(int *resources)
{
    char *names[COUNT] = {"food", "linemate", "deraumere", "sibur", "mendiane",
        "phiras", "thystame"};
    char *res = strdup("");
    char *temp;

    for (int i = 0; i < COUNT; ++i) {
        temp = repeat_word(names[i], resources[i]);
        if (strlen(res) && strlen(temp)) {
            res = realloc(res, strlen(res) + strlen(temp) + 2);
            strcat(res, " ");
        } else
            res = realloc(res, strlen(res) + strlen(temp) + 1);
        strcat(res, temp);
        free(temp);
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
