/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_inventory_utils
*/

#include "../include/player.h"
#include <stdlib.h>
#include <string.h>

char *get_resource_name(resource_type_t type)
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

bool inventory_has_item(player_t *player, resource_type_t type)
{
    if (!player || type >= COUNT)
        return false;
    return player->inventory[type] > 0;
}

int how_many_in_inventory(player_t *player, resource_type_t type)
{
    if (!player || type >= COUNT)
        return 0;
    if (!player->inventory[type])
        return 0;
    return player->inventory[type];
}
