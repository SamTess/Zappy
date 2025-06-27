/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_resources_map - tests for resources_map functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/tile.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/parsing.h"
#include <stdlib.h>
#include <string.h>

// Test helper functions
static server_t *create_test_server(int width, int height) {
    server_t *server = calloc(1, sizeof(server_t));
    server->parsed_info = calloc(1, sizeof(parsing_info_t));
    server->parsed_info->width = width;
    server->parsed_info->height = height;
    return server;
}

static tile_t **create_test_map(int width, int height) {
    tile_t **map = calloc(height, sizeof(tile_t *));
    for (int i = 0; i < height; i++) {
        map[i] = calloc(width, sizeof(tile_t));
        for (int j = 0; j < width; j++) {
            tile_init(&map[i][j]);
        }
    }
    return map;
}

static void free_test_map(tile_t **map, int width, int height) {
    if (!map) return;
    for (int i = 0; i < height; i++) {
        if (map[i]) {
            for (int j = 0; j < width; j++) {
                tile_free(&map[i][j]);
            }
            free(map[i]);
        }
    }
    free(map);
}

static void free_test_server(server_t *server) {
    if (!server) return;
    if (server->parsed_info) free(server->parsed_info);
    free(server);
}

// Test cases
Test(resources_map, distribute_resources_basic) {
    server_t *server = create_test_server(10, 10);
    tile_t **map = create_test_map(10, 10);
    int total_resources[COUNT] = {0};
    int current_resources[COUNT] = {0};

    distribute_resources(map, server, total_resources, current_resources);

    // Check that resources were distributed
    int total_food = 0;
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            total_food += map[y][x].resources[FOOD];
        }
    }

    cr_assert_gt(total_food, 0, "Food should be distributed on the map");
    cr_assert_gt(total_resources[FOOD], 0, "Total food count should be positive");

    free_test_map(map, 10, 10);
    free_test_server(server);
}

Test(resources_map, distribute_resources_density) {
    server_t *server = create_test_server(10, 10);
    tile_t **map = create_test_map(10, 10);
    int total_resources[COUNT] = {0};
    int current_resources[COUNT] = {0};

    distribute_resources(map, server, total_resources, current_resources);

    // Food should be most abundant (density 0.5)
    // Thystame should be least abundant (density 0.05)
    cr_assert_gt(total_resources[FOOD], total_resources[THYSTAME],
                 "Food should be more abundant than thystame");

    free_test_map(map, 10, 10);
    free_test_server(server);
}

Test(resources_map, respawn_resources_basic) {
    server_t *server = create_test_server(5, 5);
    tile_t **map = create_test_map(5, 5);
    int total_resources[COUNT] = {50, 15, 7, 5, 5, 4, 2}; // Expected values for 5x5 map
    int current_resources[COUNT] = {25, 7, 3, 2, 2, 2, 1}; // Half consumed

    respawn_resources(map, server, total_resources, current_resources);

    // Check that missing resources were added back
    int total_food_after = 0;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            total_food_after += map[y][x].resources[FOOD];
        }
    }

    cr_assert_gt(total_food_after, 0, "Resources should be respawned");

    free_test_map(map, 5, 5);
    free_test_server(server);
}

Test(resources_map, respawn_no_missing_resources) {
    server_t *server = create_test_server(3, 3);
    tile_t **map = create_test_map(3, 3);
    int total_resources[COUNT] = {10, 5, 3, 2, 2, 1, 1};
    int current_resources[COUNT] = {10, 5, 3, 2, 2, 1, 1}; // No missing resources

    // Count initial resources
    int initial_food = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            initial_food += map[y][x].resources[FOOD];
        }
    }

    respawn_resources(map, server, total_resources, current_resources);

    // Count final resources - should be the same since nothing was missing
    int final_food = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            final_food += map[y][x].resources[FOOD];
        }
    }

    cr_assert_eq(final_food, initial_food, "No resources should be added when none are missing");

    free_test_map(map, 3, 3);
    free_test_server(server);
}