/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_player_inventory_funcs - Complete coverage tests for player inventory functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/player.h"
#include "../src/Server/include/parsing.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

TestSuite(player_inventory_funcs);

// Test init_inventory function - currently 0% coverage
Test(player_inventory_funcs, test_init_inventory_valid_player)
{
    player_t *player = calloc(1, sizeof(player_t));
    
    // Initialize inventory - this should set all items to 0 except FOOD = 10
    init_inventory(player);
    
    // Verify FOOD is set to 10
    cr_assert_eq(player->inventory[FOOD], 10, "FOOD should be initialized to 10");
    
    // Verify other resources are 0
    for (int i = 0; i < COUNT; i++) {
        if (i != FOOD) {
            cr_assert_eq(player->inventory[i], 0, "Resource %d should be initialized to 0", i);
        }
    }
    
    free(player);
}

Test(player_inventory_funcs, test_init_inventory_null_player)
{
    // Test with NULL player - should not crash
    init_inventory(NULL);
    
    cr_assert(true, "init_inventory should handle NULL player gracefully");
}

// Test free_inventory function - currently 0% coverage
Test(player_inventory_funcs, test_free_inventory_valid_player)
{
    player_t *player = calloc(1, sizeof(player_t));
    
    // Set some inventory values first
    player->inventory[FOOD] = 15;
    player->inventory[LINEMATE] = 5;
    player->inventory[DERAUMERE] = 3;
    player->inventory_size = 23;
    
    // Free inventory - should reset all to 0
    free_inventory(player);
    
    // Verify all resources are reset to 0
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(player->inventory[i], 0, "Resource %d should be reset to 0", i);
    }
    
    // Verify inventory_size is reset
    cr_assert_eq(player->inventory_size, 0, "inventory_size should be reset to 0");
    
    free(player);
}

Test(player_inventory_funcs, test_free_inventory_null_player)
{
    // Test with NULL player - should not crash
    free_inventory(NULL);
    
    cr_assert(true, "free_inventory should handle NULL player gracefully");
}

// Test edge cases for add_item_to_inventory to improve coverage
Test(player_inventory_funcs, test_add_item_edge_cases)
{
    player_t *player = calloc(1, sizeof(player_t));
    init_inventory(player);
    
    // Test with NULL player
    bool result = add_item_to_inventory(NULL, FOOD, 5);
    cr_assert_eq(result, false, "Should return false for NULL player");
    
    // Test with negative quantity
    result = add_item_to_inventory(player, FOOD, -5);
    cr_assert_eq(result, false, "Should return false for negative quantity");
    
    // Test with invalid resource type
    result = add_item_to_inventory(player, COUNT, 5);
    cr_assert_eq(result, false, "Should return false for invalid resource type");
    
    // Test with resource type beyond COUNT
    result = add_item_to_inventory(player, COUNT + 1, 5);
    cr_assert_eq(result, false, "Should return false for resource type beyond COUNT");
    
    free(player);
}

// Test edge cases for remove_item_from_inventory to improve coverage
Test(player_inventory_funcs, test_remove_item_edge_cases, .init = cr_redirect_stdout)
{
    player_t *player = calloc(1, sizeof(player_t));
    init_inventory(player);
    
    // Test with NULL player
    bool result = remove_item_from_inventory(NULL, FOOD, 5);
    cr_assert_eq(result, false, "Should return false for NULL player");
    
    // Test with zero quantity
    result = remove_item_from_inventory(player, FOOD, 0);
    cr_assert_eq(result, false, "Should return false for zero quantity");
    
    // Test with negative quantity
    result = remove_item_from_inventory(player, FOOD, -5);
    cr_assert_eq(result, false, "Should return false for negative quantity");
    
    // Test with invalid resource type
    result = remove_item_from_inventory(player, COUNT, 5);
    cr_assert_eq(result, false, "Should return false for invalid resource type");
    
    // Test removing from empty inventory (not FOOD)
    result = remove_item_from_inventory(player, LINEMATE, 1);
    cr_assert_eq(result, false, "Should return false when trying to remove from empty inventory");
    
    // Test removing more than available - this should print warning
    player->inventory[SIBUR] = 3;
    result = remove_item_from_inventory(player, SIBUR, 5);
    cr_assert_eq(result, true, "Should return true but adjust quantity");
    cr_assert_eq(player->inventory[SIBUR], 0, "Should reset to 0 when removing more than available");
    
    free(player);
}

// Test complete inventory cycle
Test(player_inventory_funcs, test_complete_inventory_cycle)
{
    player_t *player = calloc(1, sizeof(player_t));
    
    // Initialize
    init_inventory(player);
    cr_assert_eq(player->inventory[FOOD], 10, "FOOD should start at 10");
    
    // Add items
    add_item_to_inventory(player, LINEMATE, 5);
    add_item_to_inventory(player, DERAUMERE, 3);
    cr_assert_eq(player->inventory[LINEMATE], 5, "LINEMATE should be 5");
    cr_assert_eq(player->inventory[DERAUMERE], 3, "DERAUMERE should be 3");
    
    // Remove items
    remove_item_from_inventory(player, FOOD, 2);
    remove_item_from_inventory(player, LINEMATE, 1);
    cr_assert_eq(player->inventory[FOOD], 8, "FOOD should be 8 after removal");
    cr_assert_eq(player->inventory[LINEMATE], 4, "LINEMATE should be 4 after removal");
    
    // Free inventory
    free_inventory(player);
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(player->inventory[i], 0, "All resources should be 0 after free");
    }
    
    free(player);
}