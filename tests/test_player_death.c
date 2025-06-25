/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_player_death - comprehensive unit tests for player death functionality
** Tests for handle_player_death and check_player_starvation functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/player.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"
#include "../src/Server/include/command.h"
#include "test_mocks.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// External mock variables from test_mocks.c
extern int mock_write_calls;
extern int mock_tile_remove_calls;
extern char *last_message;

// Additional mock variables for player_death tests (only for non-conflicting functions)
static int mock_command_pdi_calls = 0;
static int mock_remove_fd_calls = 0;
static int mock_how_many_in_inventory_calls = 0;
static int mock_remove_item_from_inventory_calls = 0;

// Reset mock function call counters and states
static void reset_player_death_mocks(void)
{
    mock_write_calls = 0;
    mock_tile_remove_calls = 0;
    mock_command_pdi_calls = 0;
    mock_remove_fd_calls = 0;
    mock_how_many_in_inventory_calls = 0;
    mock_remove_item_from_inventory_calls = 0;
    
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Mock implementations for player_death tests (only non-conflicting ones)
void __wrap_command_pdi(server_t *server, client_t *client)
{
    (void)server;
    (void)client;
    mock_command_pdi_calls++;
}

void __wrap_remove_fd(server_t *server, int client_fd)
{
    (void)server;
    (void)client_fd;
    mock_remove_fd_calls++;
}

// These mocks will delegate to real functions to avoid conflicts
int __wrap_how_many_in_inventory(player_t *player, resource_type_t type)
{
    mock_how_many_in_inventory_calls++;
    // Call the real function
    extern int __real_how_many_in_inventory(player_t *player, resource_type_t type);
    return __real_how_many_in_inventory(player, type);
}

bool __wrap_remove_item_from_inventory(player_t *player, resource_type_t type, int quantity)
{
    mock_remove_item_from_inventory_calls++;
    // Call the real function
    extern bool __real_remove_item_from_inventory(player_t *player, resource_type_t type, int quantity);
    return __real_remove_item_from_inventory(player, type, quantity);
}

// Test suite setup
TestSuite(player_death_tests);

// =============================================================================
// HELPER FUNCTIONS
// =============================================================================

/**
 * Create a test server with specified dimensions
 */
static server_t *create_test_server(int width, int height)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = width;
    parsed_info->height = height;
    server->parsed_info = parsed_info;
    
    // Allocate and initialize map
    server->map = calloc(height, sizeof(tile_t *));
    for (int y = 0; y < height; y++) {
        server->map[y] = calloc(width, sizeof(tile_t));
        for (int x = 0; x < width; x++) {
            server->map[y][x].player_ids = calloc(4, sizeof(int));
            server->map[y][x].player_capacity = 4;
            server->map[y][x].player_count = 0;
        }
    }
    
    return server;
}

/**
 * Create a test client with specified attributes
 */
static client_t *create_test_client(int client_id, int client_fd, int pos_x, int pos_y, int life, bool is_fully_connected)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = client_id;
    client->client_fd = client_fd;
    client->player = player;
    client->is_fully_connected = is_fully_connected;
    
    player->pos_x = pos_x;
    player->pos_y = pos_y;
    player->life = life;
    player->level = 1;
    player->team_name = strdup("test_team");
    
    return client;
}

/**
 * Free test server resources
 */
static void free_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->map && server->parsed_info) {
        for (int y = 0; y < server->parsed_info->height; y++) {
            if (server->map[y]) {
                for (int x = 0; x < server->parsed_info->width; x++) {
                    free(server->map[y][x].player_ids);
                }
                free(server->map[y]);
            }
        }
        free(server->map);
    }
    
    free(server->parsed_info);
    free(server);
}

/**
 * Free test client resources
 */
static void free_test_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

// =============================================================================
// HANDLE_PLAYER_DEATH TESTS
// =============================================================================

Test(player_death_tests, test_handle_player_death_null_client)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    
    handle_player_death(server, NULL);
    
    // Should return early without any calls
    cr_assert_eq(mock_write_calls, 0);
    cr_assert_eq(mock_command_pdi_calls, 0);
    cr_assert_eq(mock_tile_remove_calls, 0);
    cr_assert_eq(mock_remove_fd_calls, 0);
    
    free_test_server(server);
}

Test(player_death_tests, test_handle_player_death_null_player)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = calloc(1, sizeof(client_t));
    client->client_id = 42;
    client->client_fd = 5;
    client->player = NULL; // NULL player
    
    handle_player_death(server, client);
    
    // Should return early without any calls
    cr_assert_eq(mock_write_calls, 0);
    cr_assert_eq(mock_command_pdi_calls, 0);
    cr_assert_eq(mock_tile_remove_calls, 0);
    cr_assert_eq(mock_remove_fd_calls, 0);
    
    free_test_server(server);
    free(client);
}

Test(player_death_tests, test_handle_player_death_valid_client_in_bounds)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, 0, true);
    
    handle_player_death(server, client);
    
    // Should call all necessary functions
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "dead\n");
    cr_assert_eq(mock_command_pdi_calls, 1);
    cr_assert_eq(mock_tile_remove_calls, 1);
    
    // Note: The real function doesn't set client->player to NULL
    // That's handled by remove_fd which may free the entire client structure
    // The player should still be accessible in this test context
    cr_assert_not_null(client->player);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_handle_player_death_position_out_of_bounds_x)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 15, 4, 0, true); // x=15 out of bounds
    
    handle_player_death(server, client);
    
    // Should call functions but skip tile removal due to out of bounds
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "dead\n");
    cr_assert_eq(mock_command_pdi_calls, 1);
    cr_assert_eq(mock_tile_remove_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_handle_player_death_position_out_of_bounds_y)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 15, 0, true); // y=15 out of bounds
    
    handle_player_death(server, client);
    
    // Should call functions but skip tile removal due to out of bounds
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "dead\n");
    cr_assert_eq(mock_command_pdi_calls, 1);
    cr_assert_eq(mock_tile_remove_calls, 0); // Should not be called
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_handle_player_death_null_server_map)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    free(server->map); // Free the map to simulate NULL map
    server->map = NULL;
    
    client_t *client = create_test_client(42, 5, 3, 4, 0, true);
    
    handle_player_death(server, client);
    
    // Should call functions but skip tile removal due to NULL map
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "dead\n");
    cr_assert_eq(mock_command_pdi_calls, 1);
    cr_assert_eq(mock_tile_remove_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_handle_player_death_boundary_positions)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    
    // Test position at (0,0) - should be valid
    client_t *client = create_test_client(42, 5, 0, 0, 0, true);
    
    handle_player_death(server, client);
    
    cr_assert_eq(mock_tile_remove_calls, 1); // Should be called for valid position
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_handle_player_death_max_boundary_positions)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    
    // Test position at (9,9) - should be valid (last valid position)
    client_t *client = create_test_client(42, 5, 9, 9, 0, true);
    
    handle_player_death(server, client);
    
    cr_assert_eq(mock_tile_remove_calls, 1); // Should be called for valid position
    
    free_test_server(server);
    free_test_client(client);
}

// =============================================================================
// CHECK_PLAYER_STARVATION TESTS
// =============================================================================

Test(player_death_tests, test_check_player_starvation_null_client)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    
    bool result = check_player_starvation(server, NULL);
    
    // Should return false (not dead) and make no calls
    cr_assert_eq(result, false);
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
}

Test(player_death_tests, test_check_player_starvation_null_player)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = calloc(1, sizeof(client_t));
    client->client_id = 42;
    client->player = NULL;
    client->is_fully_connected = true;
    
    bool result = check_player_starvation(server, client);
    
    // Should return false (not dead) and make no calls
    cr_assert_eq(result, false);
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
    free(client);
}

Test(player_death_tests, test_check_player_starvation_not_fully_connected)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, 50, false); // not fully connected
    
    bool result = check_player_starvation(server, client);
    
    // Should return false (not dead) and make no calls
    cr_assert_eq(result, false);
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_check_player_starvation_life_above_zero)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, 50, true); // life = 50
    
    bool result = check_player_starvation(server, client);
    
    // Should decrease life but not trigger starvation
    cr_assert_eq(result, false);
    cr_assert_eq(client->player->life, 49); // Life should be decremented
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_check_player_starvation_life_becomes_zero)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, 1, true); // life = 1, will become 0
    
    // Initialize inventory with food
    client->player->inventory[FOOD] = 5;
    
    bool result = check_player_starvation(server, client);
    
    // Should consume food and restore life
    cr_assert_eq(result, false);
    cr_assert_eq(client->player->life, 126); // MAX_LIFE_AFTER_FOOD
    cr_assert_eq(client->player->inventory[FOOD], 4); // Food should be consumed
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_check_player_starvation_no_food_death)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, 1, true); // life = 1, will become 0
    
    // Initialize inventory with no food
    client->player->inventory[FOOD] = 0;
    
    bool result = check_player_starvation(server, client);
    
    // Should trigger death
    cr_assert_eq(result, true);
    
    free_test_server(server);
    // Note: Client cleanup is handled by real functions now
    free_test_client(client);
}

Test(player_death_tests, test_check_player_starvation_already_zero_life_with_food)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, 0, true); // life = 0
    
    // Initialize inventory with food
    client->player->inventory[FOOD] = 3;
    
    bool result = check_player_starvation(server, client);
    
    // Should consume food and restore life
    cr_assert_eq(result, false);
    cr_assert_eq(client->player->life, 126); // MAX_LIFE_AFTER_FOOD
    cr_assert_eq(client->player->inventory[FOOD], 2); // Food should be consumed
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_check_player_starvation_negative_life_with_food)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, -5, true); // life = -5
    
    // Initialize inventory with food
    client->player->inventory[FOOD] = 2;
    
    bool result = check_player_starvation(server, client);
    
    // Life will be decremented to -6, still <= 0, should consume food
    cr_assert_eq(result, false);
    cr_assert_eq(client->player->life, 126); // MAX_LIFE_AFTER_FOOD
    cr_assert_eq(client->player->inventory[FOOD], 1); // Food should be consumed
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_check_player_starvation_multiple_calls_progression)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, 5, 3, 4, 3, true); // life = 3
    
    // First call: life 3 -> 2
    bool result1 = check_player_starvation(server, client);
    cr_assert_eq(result1, false);
    cr_assert_eq(client->player->life, 2);
    
    // Second call: life 2 -> 1  
    bool result2 = check_player_starvation(server, client);
    cr_assert_eq(result2, false);
    cr_assert_eq(client->player->life, 1);
    
    // Third call: life 1 -> 0, no food = death
    client->player->inventory[FOOD] = 0; // No food
    bool result3 = check_player_starvation(server, client);
    cr_assert_eq(result3, true);
    
    free_test_server(server);
    free_test_client(client);
}

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

Test(player_death_tests, test_starvation_to_death_integration)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(5, 5);
    client_t *client = create_test_client(100, 10, 2, 2, 1, true);
    
    // Initialize inventory with no food
    client->player->inventory[FOOD] = 0;
    
    // Trigger starvation which should lead to death
    bool is_dead = check_player_starvation(server, client);
    
    // Verify death occurred
    cr_assert_eq(is_dead, true);
    
    // Verify all death-related functions were called
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "dead\n");
    cr_assert_eq(mock_command_pdi_calls, 1);
    cr_assert_eq(mock_tile_remove_calls, 1);
    
    free_test_server(server);
}

Test(player_death_tests, test_edge_case_zero_dimensions_map)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(0, 0); // Zero dimensions
    client_t *client = create_test_client(42, 5, 0, 0, 0, true);
    
    handle_player_death(server, client);
    
    // Should still call cleanup functions but skip tile removal
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_eq(mock_command_pdi_calls, 1);
    cr_assert_eq(mock_tile_remove_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_death_tests, test_large_map_valid_positions)
{
    reset_player_death_mocks();
    
    server_t *server = create_test_server(1000, 1000); // Large map
    client_t *client = create_test_client(42, 5, 999, 999, 0, true); // Max valid position
    
    handle_player_death(server, client);
    
    // Should call all functions including tile removal
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_eq(mock_command_pdi_calls, 1);
    cr_assert_eq(mock_tile_remove_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}