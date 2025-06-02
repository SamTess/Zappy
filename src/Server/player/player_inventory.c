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
    add_item_to_inventory(player, FOOD, 10);
    add_item_to_inventory(player, LINEMATE, 0);
    add_item_to_inventory(player, DERAUMERE, 0);
    add_item_to_inventory(player, SIBUR, 0);
    add_item_to_inventory(player, MENDIANE, 0);
    add_item_to_inventory(player, PHIRAS, 0);
    add_item_to_inventory(player, THYSTAME, 0);
}

void free_inventory(player_t *player)
{
    if (!player || !player->inventory)
        return;
    for (int i = 0; i < player->inventory_size; i++) {
        free(player->inventory[i].name);
    }
    free(player->inventory);
    player->inventory = NULL;
    player->inventory_size = 0;
}

static bool add_quantity_to_item(player_t *player, ResourceType_t type, int nb)
{
    for (int i = 0; i < player->inventory_size; i++) {
        if (player->inventory[i].type == type) {
            player->inventory[i].quantity += nb;
            return true;
        }
    }
    return false;
}

static void add_one_to_inventory(player_t *player, ResourceType_t type, int nb)
{
    player_inventory_t *new_item = malloc(sizeof(player_inventory_t));

    new_item->type = type;
    new_item->name = get_resource_name(type);
    if (!new_item->name) {
        free(new_item);
        return;
    }
    new_item->quantity = nb;
    player->inventory_size++;
    player->inventory = realloc(player->inventory,
        sizeof(player_inventory_t) * (player->inventory_size));
    if (!player->inventory) {
        free(new_item->name);
        free(new_item);
        return;
    }
    player->inventory[player->inventory_size - 1] = *new_item;
}

bool add_item_to_inventory(player_t *player, ResourceType_t type, int quantity)
{
    if (!player || quantity < 0)
        return false;
    if (inventory_has_item(player, type))
        return add_quantity_to_item(player, type, quantity);
    add_one_to_inventory(player, type, quantity);
    return true;
}

static void item_to_remove_find(player_t *player, int quantity, int i)
{
        if (player->inventory[i].quantity < quantity)
            printf("Warning: Not enough quantity to remove, remove all\n");
        player->inventory[i].quantity -= quantity;
        if (player->inventory[i].quantity < 0)
            player->inventory[i].quantity = 0;
}

bool remove_item_from_inventory(player_t *player, ResourceType_t type, int nb)
{
    if (!player || nb <= 0)
        return false;
    if (!inventory_has_item(player, type)) {
        printf("Item not found in inventory.\n");
        return false;
    }
    for (int i = 0; i < player->inventory_size; i++) {
        if (player->inventory[i].type == type) {
            item_to_remove_find(player, nb, i);
            return true;
        }
    }
    return false;
}
