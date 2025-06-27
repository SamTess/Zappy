/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_tile - tests for tile functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/tile.h"
#include "../src/Server/include/server.h"
#include <stdlib.h>
#include <string.h>

TestSuite(tile_tests);

// Test tile_init function
Test(tile_tests, test_tile_init)
{
    tile_t tile;
    
    // Initialize with some garbage values
    tile.player_ids = (int*)0xDEADBEEF;
    tile.player_count = 999;
    tile.player_capacity = 888;
    tile.egg_ids = (int*)0xCAFEBABE;
    tile.egg_count = 777;
    tile.egg_capacity = 666;
    for (int i = 0; i < COUNT; i++) {
        tile.resources[i] = 555;
    }
    
    tile_init(&tile);
    
    // Check that everything is properly initialized
    cr_assert_null(tile.player_ids, "player_ids should be NULL");
    cr_assert_eq(tile.player_count, 0, "player_count should be 0");
    cr_assert_eq(tile.player_capacity, 0, "player_capacity should be 0");
    cr_assert_null(tile.egg_ids, "egg_ids should be NULL");
    cr_assert_eq(tile.egg_count, 0, "egg_count should be 0");
    cr_assert_eq(tile.egg_capacity, 0, "egg_capacity should be 0");
    
    // Check that all resources are zeroed
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(tile.resources[i], 0, "All resources should be 0");
    }
}

// Test tile_free function
Test(tile_tests, test_tile_free)
{
    tile_t tile;
    tile_init(&tile);
    
    // Allocate some memory
    tile.player_ids = malloc(sizeof(int) * 4);
    tile.player_capacity = 4;
    tile.player_count = 2;
    tile.player_ids[0] = 1;
    tile.player_ids[1] = 2;
    
    tile.egg_ids = malloc(sizeof(int) * 3);
    tile.egg_capacity = 3;
    tile.egg_count = 1;
    tile.egg_ids[0] = 10;
    
    tile_free(&tile);
    
    // Check that everything is properly freed and reset
    cr_assert_null(tile.player_ids, "player_ids should be NULL after free");
    cr_assert_eq(tile.player_count, 0, "player_count should be 0 after free");
    cr_assert_eq(tile.player_capacity, 0, "player_capacity should be 0 after free");
    cr_assert_null(tile.egg_ids, "egg_ids should be NULL after free");
    cr_assert_eq(tile.egg_count, 0, "egg_count should be 0 after free");
    cr_assert_eq(tile.egg_capacity, 0, "egg_capacity should be 0 after free");
}

// Test resources initialization and modification
Test(tile_tests, test_tile_resources)
{
    tile_t tile;
    
    // Set some garbage values
    for (int i = 0; i < COUNT; i++) {
        tile.resources[i] = 999;
    }
    
    tile_init(&tile);
    
    // Check all resources are zeroed
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(tile.resources[i], 0, "Resource %d should be 0", i);
    }
    
    // Test that we can modify resources
    tile.resources[FOOD] = 10;
    tile.resources[LINEMATE] = 5;
    tile.resources[DERAUMERE] = 3;
    tile.resources[SIBUR] = 2;
    tile.resources[MENDIANE] = 1;
    tile.resources[PHIRAS] = 4;
    tile.resources[THYSTAME] = 6;
    
    cr_assert_eq(tile.resources[FOOD], 10, "Food should be 10");
    cr_assert_eq(tile.resources[LINEMATE], 5, "Linemate should be 5");
    cr_assert_eq(tile.resources[DERAUMERE], 3, "Deraumere should be 3");
    cr_assert_eq(tile.resources[SIBUR], 2, "Sibur should be 2");
    cr_assert_eq(tile.resources[MENDIANE], 1, "Mendiane should be 1");
    cr_assert_eq(tile.resources[PHIRAS], 4, "Phiras should be 4");
    cr_assert_eq(tile.resources[THYSTAME], 6, "Thystame should be 6");
    
    tile_free(&tile);
}

// Test tile structure consistency
Test(tile_tests, test_tile_structure_consistency)
{
    tile_t tile;
    tile_init(&tile);
    
    // Test that the structure maintains consistency
    cr_assert_eq(tile.player_count, 0, "Initial player count should be 0");
    cr_assert_eq(tile.player_capacity, 0, "Initial player capacity should be 0");
    cr_assert_eq(tile.egg_count, 0, "Initial egg count should be 0");
    cr_assert_eq(tile.egg_capacity, 0, "Initial egg capacity should be 0");
    
    // Manually simulate some operations
    tile.player_ids = malloc(sizeof(int) * 2);
    tile.player_capacity = 2;
    tile.player_count = 2;
    tile.player_ids[0] = 100;
    tile.player_ids[1] = 200;
    
    // Verify the data is stored correctly
    cr_assert_eq(tile.player_ids[0], 100, "First player should be 100");
    cr_assert_eq(tile.player_ids[1], 200, "Second player should be 200");
    cr_assert_eq(tile.player_count, 2, "Player count should be 2");
    cr_assert_eq(tile.player_capacity, 2, "Player capacity should be 2");
    
    tile_free(&tile);
}

// Test multiple init/free cycles
Test(tile_tests, test_tile_multiple_cycles)
{
    tile_t tile;
    
    // Test multiple init/free cycles
    for (int cycle = 0; cycle < 5; cycle++) {
        tile_init(&tile);
        
        // Add some data
        tile.resources[FOOD] = cycle * 10;
        tile.resources[LINEMATE] = cycle * 5;
        
        // Allocate some memory
        if (cycle > 0) {
            tile.player_ids = malloc(sizeof(int) * cycle);
            tile.player_capacity = cycle;
            tile.player_count = cycle;
            for (int i = 0; i < cycle; i++) {
                tile.player_ids[i] = i + cycle * 100;
            }
        }
        
        // Verify data
        cr_assert_eq(tile.resources[FOOD], cycle * 10, "Food should match cycle");
        cr_assert_eq(tile.resources[LINEMATE], cycle * 5, "Linemate should match cycle");
        
        tile_free(&tile);
        
        // Verify cleanup
        cr_assert_null(tile.player_ids, "player_ids should be NULL after free");
        cr_assert_eq(tile.player_count, 0, "player_count should be 0 after free");
    }
}

// Test with different resource configurations
Test(tile_tests, test_tile_resource_configurations)
{
    tile_t tile;
    tile_init(&tile);
    
    // Test maximum values
    for (int i = 0; i < COUNT; i++) {
        tile.resources[i] = 999999;
        cr_assert_eq(tile.resources[i], 999999, "Resource %d should handle large values", i);
    }
    
    // Test zero values
    for (int i = 0; i < COUNT; i++) {
        tile.resources[i] = 0;
        cr_assert_eq(tile.resources[i], 0, "Resource %d should be 0", i);
    }
    
    // Test negative values (should work as they're just ints)
    for (int i = 0; i < COUNT; i++) {
        tile.resources[i] = -100;
        cr_assert_eq(tile.resources[i], -100, "Resource %d should handle negative values", i);
    }
    
    tile_free(&tile);
}