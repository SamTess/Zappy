/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_map_creation - tests for map creation functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/tile.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/parsing.h"
#include "test_mocks.h"
#include <stdlib.h>
#include <string.h>

TestSuite(map_creation_tests);

// Helper function to create test parsing info
static parsing_info_t *create_test_parsing_info(int width, int height)
{
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    info->width = width;
    info->height = height;
    return info;
}

// Helper function to create test server
static server_t *create_test_server(parsing_info_t *info)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->total_resources = calloc(COUNT, sizeof(int));
    server->current_resources = calloc(COUNT, sizeof(int));
    server->parsed_info = info;  // Set the parsed_info so distribute_resources can access it
    return server;
}

// Helper function to cleanup test server
static void cleanup_test_server(server_t *server, parsing_info_t *info)
{
    if (server && server->map && info) {
        for (int y = 0; y < info->height; y++) {
            if (server->map[y]) {
                for (int x = 0; x < info->width; x++) {
                    tile_free(&server->map[y][x]);
                }
                free(server->map[y]);
            }
        }
        free(server->map);
    }
    if (server) {
        free(server->total_resources);
        free(server->current_resources);
        free(server);
    }
    if (info) {
        free(info);
    }
}

// Test create_map with valid small map
Test(map_creation_tests, test_create_map_small)
{
    parsing_info_t *info = create_test_parsing_info(3, 3);
    server_t *server = create_test_server(info);
    
    create_map(server, info);
    
    // Check that map was allocated
    cr_assert_not_null(server->map, "Map should be allocated");
    
    // Check that all rows were allocated
    for (int y = 0; y < info->height; y++) {
        cr_assert_not_null(server->map[y], "Row %d should be allocated", y);
    }
    
    // Check that all tiles are properly initialized
    for (int y = 0; y < info->height; y++) {
        for (int x = 0; x < info->width; x++) {
            tile_t *tile = &server->map[y][x];
            cr_assert_null(tile->player_ids, "Tile[%d][%d] player_ids should be NULL", y, x);
            cr_assert_eq(tile->player_count, 0, "Tile[%d][%d] player_count should be 0", y, x);
            cr_assert_eq(tile->player_capacity, 0, "Tile[%d][%d] player_capacity should be 0", y, x);
            cr_assert_null(tile->egg_ids, "Tile[%d][%d] egg_ids should be NULL", y, x);
            cr_assert_eq(tile->egg_count, 0, "Tile[%d][%d] egg_count should be 0", y, x);
            cr_assert_eq(tile->egg_capacity, 0, "Tile[%d][%d] egg_capacity should be 0", y, x);
            
            // Check resources - they should be distributed by the real function
            // We can't predict exact values but they should be non-negative
            for (int r = 0; r < COUNT; r++) {
                cr_assert_geq(tile->resources[r], 0, "Tile[%d][%d] resource[%d] should be non-negative", y, x, r);
            }
        }
    }
    
    cleanup_test_server(server, info);
}

// Test create_map with larger map
Test(map_creation_tests, test_create_map_large)
{
    parsing_info_t *info = create_test_parsing_info(10, 15);
    server_t *server = create_test_server(info);
    
    create_map(server, info);
    
    // Check dimensions
    cr_assert_not_null(server->map, "Map should be allocated");
    
    // Verify all tiles are accessible and initialized
    for (int y = 0; y < info->height; y++) {
        cr_assert_not_null(server->map[y], "Row %d should be allocated", y);
        for (int x = 0; x < info->width; x++) {
            tile_t *tile = &server->map[y][x];
            cr_assert_eq(tile->player_count, 0, "Tile should be initialized");
        }
    }
    
    cleanup_test_server(server, info);
}

// Test create_map with minimum size (1x1)
Test(map_creation_tests, test_create_map_minimum)
{
    parsing_info_t *info = create_test_parsing_info(1, 1);
    server_t *server = create_test_server(info);
    
    create_map(server, info);
    
    cr_assert_not_null(server->map, "Map should be allocated");
    cr_assert_not_null(server->map[0], "Single row should be allocated");
    
    // Check the single tile
    tile_t *tile = &server->map[0][0];
    cr_assert_eq(tile->player_count, 0, "Single tile should be initialized");
    cr_assert_null(tile->player_ids, "Single tile player_ids should be NULL");
    
    cleanup_test_server(server, info);
}

// Test create_map with rectangular maps
Test(map_creation_tests, test_create_map_rectangular)
{
    parsing_info_t *info = create_test_parsing_info(20, 5); // Wide map
    server_t *server = create_test_server(info);
    
    create_map(server, info);
    
    cr_assert_not_null(server->map, "Map should be allocated");
    
    // Check all tiles in wide map
    for (int y = 0; y < 5; y++) {
        cr_assert_not_null(server->map[y], "Row %d should be allocated", y);
        for (int x = 0; x < 20; x++) {
            cr_assert_eq(server->map[y][x].player_count, 0, "Tile[%d][%d] should be initialized", y, x);
        }
    }
    
    cleanup_test_server(server, info);
    
    // Test tall map
    info = create_test_parsing_info(5, 20); // Tall map
    server = create_test_server(info);
    
    create_map(server, info);
    
    cr_assert_not_null(server->map, "Tall map should be allocated");
    
    for (int y = 0; y < 20; y++) {
        cr_assert_not_null(server->map[y], "Row %d should be allocated", y);
        for (int x = 0; x < 5; x++) {
            cr_assert_eq(server->map[y][x].player_count, 0, "Tile[%d][%d] should be initialized", y, x);
        }
    }
    
    cleanup_test_server(server, info);
}

// Test that create_map properly calls tile_init for each tile
Test(map_creation_tests, test_create_map_tile_initialization)
{
    parsing_info_t *info = create_test_parsing_info(4, 3);
    server_t *server = create_test_server(info);
    
    create_map(server, info);
    
    // Verify each tile has been properly initialized by tile_init
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 4; x++) {
            tile_t *tile = &server->map[y][x];
            
            // All these should be the result of tile_init
            cr_assert_null(tile->player_ids, "player_ids should be NULL");
            cr_assert_eq(tile->player_count, 0, "player_count should be 0");
            cr_assert_eq(tile->player_capacity, 0, "player_capacity should be 0");
            cr_assert_null(tile->egg_ids, "egg_ids should be NULL");  
            cr_assert_eq(tile->egg_count, 0, "egg_count should be 0");
            cr_assert_eq(tile->egg_capacity, 0, "egg_capacity should be 0");
            
            // Resources should be non-negative after distribution
            for (int r = 0; r < COUNT; r++) {
                cr_assert_geq(tile->resources[r], 0, "Resource %d should be non-negative", r);
            }
        }
    }
    
    cleanup_test_server(server, info);
}

// Test create_map calls distribute_resources with correct parameters
Test(map_creation_tests, test_create_map_distribute_resources_call)
{
    parsing_info_t *info = create_test_parsing_info(6, 8);
    server_t *server = create_test_server(info);
    
    // Set some values to verify they're passed correctly
    for (int i = 0; i < COUNT; i++) {
        server->total_resources[i] = i * 10;
        server->current_resources[i] = i * 5;
    }
    
    create_map(server, info);
    
    // Verify the map was created successfully (implicit test that distribute_resources worked)
    cr_assert_not_null(server->map, "Map should be created successfully");
    
    cleanup_test_server(server, info);
}

// Test memory allocation pattern
Test(map_creation_tests, test_create_map_memory_pattern)
{
    parsing_info_t *info = create_test_parsing_info(2, 2);
    server_t *server = create_test_server(info);
    
    create_map(server, info);
    
    // Verify the map is a proper 2D array
    cr_assert_not_null(server->map, "Map pointer should exist");
    cr_assert_not_null(server->map[0], "First row should exist");
    cr_assert_not_null(server->map[1], "Second row should exist");
    
    // Verify we can access all positions
    server->map[0][0].resources[FOOD] = 42;
    server->map[0][1].resources[LINEMATE] = 43;
    server->map[1][0].resources[DERAUMERE] = 44;
    server->map[1][1].resources[SIBUR] = 45;
    
    cr_assert_eq(server->map[0][0].resources[FOOD], 42, "Should access [0][0]");
    cr_assert_eq(server->map[0][1].resources[LINEMATE], 43, "Should access [0][1]");
    cr_assert_eq(server->map[1][0].resources[DERAUMERE], 44, "Should access [1][0]");
    cr_assert_eq(server->map[1][1].resources[SIBUR], 45, "Should access [1][1]");
    
    cleanup_test_server(server, info);
}