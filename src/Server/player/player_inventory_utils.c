/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_inventory_utils
*/

#include "../include/player.h"
#include <stdlib.h>
#include <string.h>

char *get_resource_name(ResourceType_t type)
{
    switch (type) {
        case FOOD:
            return strdup("food");
        case LINEMATE:
            return strdup("linemate");
        case DERAUMERE:
            return strdup("deraumere");
        case SIBUR:
            return strdup("sibur");
        case MENDIANE:
            return strdup("mendiane");
        case PHIRAS:
            return strdup("phiras");
        case THYSTAME:
            return strdup("thystame");
        default:
            return NULL;
    }
}

bool inventory_has_item(player_t *player, ResourceType_t type)
{
    if (!player || !player->inventory)
        return false;
    for (int i = 0; i < player->inventory_size; i++) {
        if (player->inventory[i].type == type)
            return true;
    }
    return false;
}

int how_many_in_inventory(player_t *player, ResourceType_t type)
{
    if (!player)
        return 0;
    for (int i = 0; i < player->inventory_size; i++) {
        if (player->inventory[i].type == type)
            return player->inventory[i].quantity;
    }
    return 0;
}
