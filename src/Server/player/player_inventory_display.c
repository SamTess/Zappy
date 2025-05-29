/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_inventory_display
*/

#include "../include/player.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char *get_one_item_content(player_inventory_t item)
{
    int total_length = strlen(item.name) +
        snprintf(NULL, 0, "%d", item.quantity) + 3;
    char *content = malloc(total_length);

    if (!content)
        return NULL;
    snprintf(content, total_length  , "%s %d", item.name, item.quantity);
    return content;
}

static void prosses_one_item(player_inventory_t item, char **content,
    size_t *content_length, bool is_last)
{
    char *item_str = get_one_item_content(item);
    size_t item_length = 0;

    if (!item_str)
        return;
    item_length = strlen(item_str);
    *content_length += item_length + 2;
    *content = realloc(*content, *content_length);
    if (!*content) {
        free(item_str);
        return;
    }
    strcat(*content, item_str);
    if (!is_last)
        strcat(*content, ", ");
    else
        strcat(*content, "]\n");
    free(item_str);
}

char *get_inventory_content(player_t *player)
{
    char *content = strdup("[");
    size_t content_length = 1;

    if (!player || player->inventory_size == 0)
        return strdup("[]");
    for (int i = 0; i < player->inventory_size; i++) {
        if (i == player->inventory_size - 1)
            prosses_one_item(player->inventory[i], &content,
                &content_length, true);
        else
            prosses_one_item(player->inventory[i], &content,
                &content_length, false);
        if (!content)
            return NULL;
    }
    return content;
}
