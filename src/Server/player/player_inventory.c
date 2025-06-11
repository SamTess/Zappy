/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_inventory
*/

#include "../include/player.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_inventory(player_t *player)
{
    if (!player)
        return;
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 0;
    }
    player->inventory[FOOD] = 10;
}

void free_inventory(player_t *player)
{
    if (!player)
        return;
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 0;
    }
    player->inventory_size = 0;
}

bool add_item_to_inventory(player_t *player, resource_type_t type, int q)
{
    if (!player || q < 0 || type >= COUNT)
        return false;
    player->inventory[type] += q;
    return true;
}

bool remove_item_from_inventory(player_t *player, resource_type_t type, int nb)
{
    if (!player || nb <= 0 || type >= COUNT)
        return false;
    if (player->inventory[type] == 0) {
        printf("Item not found in inventory.\n");
        return false;
    }
    if (player->inventory[type] < nb)
        printf("Warning: Not enough quantity to remove, remove all\n");
    player->inventory[type] -= nb;
    if (player->inventory[type] < 0)
        player->inventory[type] = 0;
    return true;
}
