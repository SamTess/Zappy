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
    if (!player || quantity <= 0)
        return false;
    if (inventory_has_item(player, type))
        return add_quantity_to_item(player, type, quantity);
    add_one_to_inventory(player, type, quantity);
    return true;
}

static void remove_one_item_from_inventory(player_t *player, int i)
{
    void *tmp;

    free(player->inventory[i].name);
    for (int j = i; j < player->inventory_size - 1; j++)
        player->inventory[j] = player->inventory[j + 1];
    player->inventory_size--;
    if (player->inventory_size > 0) {
        tmp = realloc(player->inventory,
            sizeof(player_inventory_t) * player->inventory_size);
        if (!tmp) {
            perror("Error: Memory allocation failed for inventory");
            exit(84);
        }
        player->inventory = tmp;
    } else {
        free(player->inventory);
        player->inventory = NULL;
    }
}

static void item_to_remove_find(player_t *player, int quantity, int i)
{
        if (player->inventory[i].quantity < quantity)
            printf("Warning: Not enough quantity to remove, remove all\n");
        player->inventory[i].quantity -= quantity;
        if (player->inventory[i].quantity <= 0)
            remove_one_item_from_inventory(player, i);
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
