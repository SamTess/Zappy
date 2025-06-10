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

static char *get_one_item_content(resource_type_t type, int quantity)
{
    char *name = get_resource_name(type);
    int total_length = strlen(name) + snprintf(NULL, 0, "%d", quantity) + 3;
    char *content = malloc(total_length);

    if (!content) {
        free(name);
        return NULL;
    }
    snprintf(content, total_length, "%s %d", name, quantity);
    free(name);
    return content;
}

static bool append_item_string(char **content, size_t *content_length,
    const char *item_str)
{
    size_t item_length = strlen(item_str);

    *content_length += item_length + 3;
    *content = realloc(*content, *content_length);
    if (!*content)
        return false;
    strcat(*content, item_str);
    return true;
}

static void add_separator(char **content, bool is_last)
{
    if (!is_last)
        strcat(*content, ", ");
    else
        strcat(*content, "]\n");
}

static void process_one_item(resource_type_t type, int quantity,
    char **content, size_t *content_length)
{
    char *item_str = get_one_item_content(type, quantity);

    if (!item_str)
        return;
    if (!append_item_string(content, content_length, item_str)) {
        free(item_str);
        return;
    }
    free(item_str);
}

char *get_inventory_content(player_t *player)
{
    char *content = strdup("[");
    size_t content_length = 1;
    int quantity;

    if (!player)
        return strdup("[]");
    for (int i = 0; i < COUNT; i++) {
        quantity = 0;
        if (player->inventory[i])
            quantity = player->inventory[i];
        process_one_item(i, quantity, &content, &content_length);
        if (!content)
            return NULL;
        add_separator(&content, i == COUNT - 1);
    }
    return content;
}
