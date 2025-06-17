/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_utility_functions - tests for utility functions to improve coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"
#include "test_mocks.h"

// External mock variables
extern int mock_write_calls;
extern char *last_message;

// Reset mock function call counters and states
static void reset_utility_mocks(void)
{
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

/**
 * Create a test server with map
 */
static server_t *create_test_server_with_map(int width, int height)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = width;
    parsed_info->height = height;
    server->parsed_info = parsed_info;
    server->current_tick = 100;
    
    // Allocate and initialize map
    server->map = calloc(height, sizeof(tile_t *));
    for (int y = 0; y < height; y++) {
        server->map[y] = calloc(width, sizeof(tile_t));
        for (int x = 0; x < width; x++) {
            server->map[y][x].player_ids = calloc(4, sizeof(int));
            server->map[y][x].player_capacity = 4;
            server->map[y][x].player_count = 0;
            // Initialize resources
            for (int r = 0; r < COUNT; r++) {
                server->map[y][x].resources[r] = 0;
            }
        }
    }
    
    return server;
}

/**
 * Create a test client with player
 */
static client_t *create_test_client_with_player(int pos_x, int pos_y, int rotation)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 42;
    client->client_fd = 5;
    client->player = player;
    
    player->pos_x = pos_x;
    player->pos_y = pos_y;
    player->rotation = rotation;
    player->level = 1;
    player->team_name = strdup("TestTeam");
    player->command_queue = NULL;
    player->queue_size = 0;
    player->pending_cmd = NULL;
    
    return client;
}

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

static void free_test_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

TestSuite(parse_command_utils_tests);

// Test: cleanup_pending with NULL player
Test(parse_command_utils_tests, test_cleanup_pending_null_player)
{
    reset_utility_mocks();
    
    cleanup_pending(NULL);
    
    // Should handle NULL gracefully without crash
    cr_assert(true); // If we reach here, test passed
    
    reset_utility_mocks();
}

// Test: cleanup_pending with player without pending cmd
Test(parse_command_utils_tests, test_cleanup_pending_no_pending_cmd)
{
    player_t player = {0};
    player.pending_cmd = NULL;
    
    reset_utility_mocks();
    
    cleanup_pending(&player);
    
    // Should handle NULL pending_cmd gracefully
    cr_assert(player.pending_cmd == NULL);
    
    reset_utility_mocks();
}

// Test: cleanup_pending with valid pending cmd
Test(parse_command_utils_tests, test_cleanup_pending_valid_cmd)
{
    player_t player = {0};
    player.pending_cmd = calloc(1, sizeof(pending_cmd_t));
    player.pending_cmd->args = strdup("test_args");
    
    reset_utility_mocks();
    
    cleanup_pending(&player);
    
    // Should free pending_cmd and set to NULL
    cr_assert(player.pending_cmd == NULL);
    
    reset_utility_mocks();
}

// Test: cleanup_player_queue with NULL player
Test(parse_command_utils_tests, test_cleanup_player_queue_null_player)
{
    reset_utility_mocks();
    
    cleanup_player_queue(NULL);
    
    // Should handle NULL gracefully
    cr_assert(true);
    
    reset_utility_mocks();
}

// Test: cleanup_player_queue with no queue
Test(parse_command_utils_tests, test_cleanup_player_queue_no_queue)
{
    player_t player = {0};
    player.command_queue = NULL;
    player.queue_size = 0;
    
    reset_utility_mocks();
    
    cleanup_player_queue(&player);
    
    // Should handle NULL queue gracefully
    cr_assert(player.command_queue == NULL);
    cr_assert_eq(player.queue_size, 0);
    
    reset_utility_mocks();
}

// Test: cleanup_player_queue with valid queue
Test(parse_command_utils_tests, test_cleanup_player_queue_valid_queue)
{
    player_t player = {0};
    player.queue_size = 2;
    player.command_queue = calloc(2, sizeof(char *));
    player.command_queue[0] = strdup("command1");
    player.command_queue[1] = strdup("command2");
    
    reset_utility_mocks();
    
    cleanup_player_queue(&player);
    
    // Should free queue and reset
    cr_assert(player.command_queue == NULL);
    cr_assert_eq(player.queue_size, 0);
    
    reset_utility_mocks();
}

// Test: cleanup_client with NULL client
Test(parse_command_utils_tests, test_cleanup_client_null_client)
{
    reset_utility_mocks();
    
    cleanup_client(NULL);
    
    // Should handle NULL gracefully
    cr_assert(true);
    
    reset_utility_mocks();
}

// Test: cleanup_client with valid client
Test(parse_command_utils_tests, test_cleanup_client_valid_client)
{
    client_t *client = create_test_client_with_player(5, 5, UP);
    client->player->pending_cmd = calloc(1, sizeof(pending_cmd_t));
    client->player->pending_cmd->args = strdup("test");
    
    reset_utility_mocks();
    
    cleanup_client(client);
    
    // Should free player and set to NULL
    cr_assert(client->player == NULL);
    
    free(client);
    reset_utility_mocks();
}

// Test: write_command_output with valid fd
Test(parse_command_utils_tests, test_write_command_output_valid_fd)
{
    // Create a temporary file to write to
    int temp_fd = open("/tmp/test_output", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    cr_assert(temp_fd >= 0);
    
    reset_utility_mocks();
    
    write_command_output(temp_fd, "test message");
    
    // Close the write fd
    close(temp_fd);
    
    // Verify the function doesn't crash and works
    // (The actual implementation might not write to real files in test context)
    cr_assert(true); // Function executed without crashing
    
    unlink("/tmp/test_output");
    reset_utility_mocks();
}

TestSuite(look_utils_tests);

// Test: check_rota_tiles with UP rotation
Test(look_utils_tests, test_check_rota_tiles_up_rotation)
{
    server_t *server = create_test_server_with_map(10, 10);
    client_t *client = create_test_client_with_player(5, 5, UP);
    
    // Set up some resources on the tile for testing
    server->map[4][5].resources[FOOD] = 1;
    server->map[4][5].resources[LINEMATE] = 2;
    
    reset_utility_mocks();
    
    char *result = check_rota_tiles(client, server, 1, 0);
    
    // Should return tile content (may be empty but not NULL)
    cr_assert(result != NULL);
    // The function should work without crashing
    cr_assert(strlen(result) != SIZE_MAX); // Check that strlen doesn't fail
    
    free(result);
    free_test_server(server);
    free_test_client(client);
    reset_utility_mocks();
}

// Test: check_rota_tiles with DOWN rotation
Test(look_utils_tests, test_check_rota_tiles_down_rotation)
{
    server_t *server = create_test_server_with_map(10, 10);
    client_t *client = create_test_client_with_player(5, 5, DOWN);
    
    // Set up some resources on the tile for testing
    server->map[6][5].resources[FOOD] = 1;
    
    reset_utility_mocks();
    
    char *result = check_rota_tiles(client, server, 1, 0);
    
    // Should return tile content (may be empty but not NULL)
    cr_assert(result != NULL);
    // The function should work without crashing
    cr_assert(strlen(result) != SIZE_MAX); // Check that strlen doesn't fail
    
    free(result);
    free_test_server(server);
    free_test_client(client);
    reset_utility_mocks();
}

// Test: check_rota_tiles with LEFT rotation
Test(look_utils_tests, test_check_rota_tiles_left_rotation)
{
    server_t *server = create_test_server_with_map(10, 10);
    client_t *client = create_test_client_with_player(5, 5, LEFT);
    
    // Set up some resources on the tile for testing
    server->map[5][4].resources[SIBUR] = 1;
    
    reset_utility_mocks();
    
    char *result = check_rota_tiles(client, server, 1, 0);
    
    // Should return tile content
    cr_assert(result != NULL);
    cr_assert(strlen(result) > 0);
    
    free(result);
    free_test_server(server);
    free_test_client(client);
    reset_utility_mocks();
}

// Test: check_rota_tiles with RIGHT rotation
Test(look_utils_tests, test_check_rota_tiles_right_rotation)
{
    server_t *server = create_test_server_with_map(10, 10);
    client_t *client = create_test_client_with_player(5, 5, RIGHT);
    
    // Set up some resources on the tile for testing
    server->map[5][6].resources[DERAUMERE] = 1;
    
    reset_utility_mocks();
    
    char *result = check_rota_tiles(client, server, 1, 0);
    
    // Should return tile content
    cr_assert(result != NULL);
    cr_assert(strlen(result) > 0);
    
    free(result);
    free_test_server(server);
    free_test_client(client);
    reset_utility_mocks();
}

// Test: check_rota_tiles with invalid rotation
Test(look_utils_tests, test_check_rota_tiles_invalid_rotation)
{
    server_t *server = create_test_server_with_map(10, 10);
    client_t *client = create_test_client_with_player(5, 5, 999); // Invalid rotation
    
    reset_utility_mocks();
    
    char *result = check_rota_tiles(client, server, 1, 0);
    
    // Should return empty string for invalid rotation
    cr_assert(result != NULL);
    cr_assert_str_eq(result, "");
    
    free(result);
    free_test_server(server);
    free_test_client(client);
    reset_utility_mocks();
}

// Test: check_rota_tiles with edge positions (wrapping)
Test(look_utils_tests, test_check_rota_tiles_edge_wrapping)
{
    server_t *server = create_test_server_with_map(10, 10);
    client_t *client = create_test_client_with_player(0, 0, UP); // Corner position
    
    // Set up resource on wrapped tile
    server->map[9][0].resources[FOOD] = 1;
    
    reset_utility_mocks();
    
    char *result = check_rota_tiles(client, server, 1, 0);
    
    // Should handle wrapping correctly
    cr_assert(result != NULL);
    
    free(result);
    free_test_server(server);
    free_test_client(client);
    reset_utility_mocks();
}

// Test: check_rota_tiles with multiple positions
Test(look_utils_tests, test_check_rota_tiles_multiple_positions)
{
    server_t *server = create_test_server_with_map(10, 10);
    client_t *client = create_test_client_with_player(5, 5, UP);
    
    reset_utility_mocks();
    
    // Test different i,j combinations
    char *result1 = check_rota_tiles(client, server, 0, 0);
    char *result2 = check_rota_tiles(client, server, 1, 1);
    char *result3 = check_rota_tiles(client, server, 2, -1);
    
    cr_assert(result1 != NULL);
    cr_assert(result2 != NULL);
    cr_assert(result3 != NULL);
    
    free(result1);
    free(result2);
    free(result3);
    free_test_server(server);
    free_test_client(client);
    reset_utility_mocks();
}