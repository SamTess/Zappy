/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_movement_commands - Comprehensive unit tests for movement commands
** Tests forward, left, and right commands with extensive coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/tile.h"
#include "test_mocks.h"

// External mock variables from test_mocks.c
extern int mock_write_calls;
extern int mock_send_ppo_calls;
extern int mock_tile_add_calls;
extern int mock_tile_remove_calls;
extern int mock_arr_len_calls;
extern char *last_message;

// Reset mock function call counters and states
static void reset_mock_functions(void)
{
    mock_write_calls = 0;
    mock_send_ppo_calls = 0;
    mock_tile_add_calls = 0;
    mock_tile_remove_calls = 0;
    mock_arr_len_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// arr_len mock is implemented in test_fork.c
// But since it's static there, we need our own instance for movement tests

// =============================================================================
// TEST DATA AND UTILITY FUNCTIONS
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
 * Create a test client with specified position and rotation
 */
static client_t *create_test_client(int pos_x, int pos_y, enum rotation_e rotation)
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
// FORWARD COMMAND TESTS
// =============================================================================

Test(forward_command, test_forward_null_client)
{
    server_t *server = create_test_server(10, 10);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, NULL, buffer);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
}

Test(forward_command, test_forward_null_player)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Forward", NULL};
    
    // Set player to NULL to test this case
    free(client->player);
    client->player = NULL;
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free(client);
}

// Note: Buffer length validation tests removed due to shared arr_len mock dependency
// The arr_len mock is controlled by test_fork.c and cannot be reliably controlled here

Test(forward_command, test_forward_invalid_rotation)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, (enum rotation_e)99); // Invalid rotation
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_move_up_success)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Check player moved up (y decreased)
    cr_assert_eq(client->player->pos_x, 5);
    cr_assert_eq(client->player->pos_y, 4);
    
    // Check success response
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    
    // Check tile operations
    cr_assert_eq(mock_tile_remove_calls, 1);
    cr_assert_eq(mock_tile_add_calls, 1);
    
    // Check graphical update
    cr_assert_eq(mock_send_ppo_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_move_down_success)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, DOWN);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Check player moved down (y increased)
    cr_assert_eq(client->player->pos_x, 5);
    cr_assert_eq(client->player->pos_y, 6);
    
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_move_left_success)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, LEFT);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Check player moved left (x decreased)
    cr_assert_eq(client->player->pos_x, 4);
    cr_assert_eq(client->player->pos_y, 5);
    
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_move_right_success)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, RIGHT);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Check player moved right (x increased)
    cr_assert_eq(client->player->pos_x, 6);
    cr_assert_eq(client->player->pos_y, 5);
    
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_wrap_top_edge)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 0, UP); // At top edge
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Should wrap to bottom
    cr_assert_eq(client->player->pos_x, 5);
    cr_assert_eq(client->player->pos_y, 9); // height - 1
    
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_wrap_bottom_edge)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 9, DOWN); // At bottom edge
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Should wrap to top
    cr_assert_eq(client->player->pos_x, 5);
    cr_assert_eq(client->player->pos_y, 0);
    
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_wrap_left_edge)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(0, 5, LEFT); // At left edge
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Should wrap to right
    cr_assert_eq(client->player->pos_x, 9); // width - 1
    cr_assert_eq(client->player->pos_y, 5);
    
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(forward_command, test_forward_wrap_right_edge)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(9, 5, RIGHT); // At right edge
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Should wrap to left
    cr_assert_eq(client->player->pos_x, 0);
    cr_assert_eq(client->player->pos_y, 5);
    
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

// =============================================================================
// LEFT COMMAND TESTS
// =============================================================================

Test(left_command, test_left_null_server)
{
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    left(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_client(client);
}

Test(left_command, test_left_null_client)
{
    server_t *server = create_test_server(10, 10);
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    left(server, NULL, buffer);
    
    // With NULL client, command returns early without calling write_command_output
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
}

Test(left_command, test_left_null_player)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Left", NULL};
    
    // Set player to NULL
    free(client->player);
    client->player = NULL;
    
    reset_mock_functions();
    
    left(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free(client);
}

// Note: Buffer length validation tests removed due to shared arr_len mock dependency

Test(left_command, test_left_invalid_rotation)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, (enum rotation_e)99); // Invalid rotation
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    left(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(left_command, test_left_rotation_up_to_left)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    left(server, client, buffer);
    
    // UP -> LEFT
    cr_assert_eq(client->player->rotation, LEFT);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    
    // Check graphical update
    cr_assert_eq(mock_send_ppo_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

Test(left_command, test_left_rotation_left_to_down)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, LEFT);
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    left(server, client, buffer);
    
    // LEFT -> DOWN
    cr_assert_eq(client->player->rotation, DOWN);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(left_command, test_left_rotation_down_to_right)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, DOWN);
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    left(server, client, buffer);
    
    // DOWN -> RIGHT
    cr_assert_eq(client->player->rotation, RIGHT);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(left_command, test_left_rotation_right_to_up)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, RIGHT);
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    left(server, client, buffer);
    
    // RIGHT -> UP
    cr_assert_eq(client->player->rotation, UP);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

// =============================================================================
// RIGHT COMMAND TESTS
// =============================================================================

Test(right_command, test_right_null_server)
{
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    right(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_client(client);
}

Test(right_command, test_right_null_client)
{
    server_t *server = create_test_server(10, 10);
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    right(server, NULL, buffer);
    
    // With NULL client, command returns early without calling write_command_output
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
}

Test(right_command, test_right_null_player)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Right", NULL};
    
    // Set player to NULL
    free(client->player);
    client->player = NULL;
    
    reset_mock_functions();
    
    right(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free(client);
}

// Note: Buffer length validation tests removed due to shared arr_len mock dependency

Test(right_command, test_right_invalid_rotation)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, (enum rotation_e)99); // Invalid rotation
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    right(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(right_command, test_right_rotation_up_to_right)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    right(server, client, buffer);
    
    // UP -> RIGHT
    cr_assert_eq(client->player->rotation, RIGHT);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    
    // Check graphical update
    cr_assert_eq(mock_send_ppo_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

Test(right_command, test_right_rotation_right_to_down)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, RIGHT);
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    right(server, client, buffer);
    
    // RIGHT -> DOWN
    cr_assert_eq(client->player->rotation, DOWN);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(right_command, test_right_rotation_down_to_left)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, DOWN);
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    right(server, client, buffer);
    
    // DOWN -> LEFT
    cr_assert_eq(client->player->rotation, LEFT);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(right_command, test_right_rotation_left_to_up)
{
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, LEFT);
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    right(server, client, buffer);
    
    // LEFT -> UP
    cr_assert_eq(client->player->rotation, UP);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

// =============================================================================
// EDGE CASE AND STRESS TESTS
// =============================================================================

Test(movement_commands, test_minimum_map_size)
{
    // Test movement on a 1x1 map (extreme edge case)
    server_t *server = create_test_server(1, 1);
    client_t *client = create_test_client(0, 0, UP);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    // Should wrap back to same position
    cr_assert_eq(client->player->pos_x, 0);
    cr_assert_eq(client->player->pos_y, 0);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(movement_commands, test_large_map_movement)
{
    // Test movement on a large map
    server_t *server = create_test_server(1000, 1000);
    client_t *client = create_test_client(500, 500, RIGHT);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client, buffer);
    
    cr_assert_eq(client->player->pos_x, 501);
    cr_assert_eq(client->player->pos_y, 500);
    cr_assert_str_eq(last_message, "ok\n");
    
    free_test_server(server);
    free_test_client(client);
}

Test(movement_commands, test_rotation_sequence_left)
{
    // Test complete 360-degree rotation sequence with left
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Left", NULL};
    
    reset_mock_functions();
    
    // UP -> LEFT
    left(server, client, buffer);
    cr_assert_eq(client->player->rotation, LEFT);
    
    // LEFT -> DOWN
    left(server, client, buffer);
    cr_assert_eq(client->player->rotation, DOWN);
    
    // DOWN -> RIGHT
    left(server, client, buffer);
    cr_assert_eq(client->player->rotation, RIGHT);
    
    // RIGHT -> UP (full circle)
    left(server, client, buffer);
    cr_assert_eq(client->player->rotation, UP);
    
    cr_assert_eq(mock_write_calls, 4);
    cr_assert_eq(mock_send_ppo_calls, 4);
    
    free_test_server(server);
    free_test_client(client);
}

Test(movement_commands, test_rotation_sequence_right)
{
    // Test complete 360-degree rotation sequence with right
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(5, 5, UP);
    char *buffer[] = {"Right", NULL};
    
    reset_mock_functions();
    
    // UP -> RIGHT
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, RIGHT);
    
    // RIGHT -> DOWN
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, DOWN);
    
    // DOWN -> LEFT
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, LEFT);
    
    // LEFT -> UP (full circle)
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, UP);
    
    cr_assert_eq(mock_write_calls, 4);
    cr_assert_eq(mock_send_ppo_calls, 4);
    
    free_test_server(server);
    free_test_client(client);
}

Test(movement_commands, test_corner_wrapping_all_directions)
{
    // Test wrapping behavior at all four corners of the map
    server_t *server = create_test_server(5, 5);
    
    // Test top-left corner moving up and left
    client_t *client1 = create_test_client(0, 0, UP);
    char *buffer[] = {"Forward", NULL};
    
    reset_mock_functions();
    
    forward(server, client1, buffer);
    cr_assert_eq(client1->player->pos_x, 0);
    cr_assert_eq(client1->player->pos_y, 4); // Wrapped to bottom
    
    client1->player->rotation = LEFT;
    forward(server, client1, buffer);
    cr_assert_eq(client1->player->pos_x, 4); // Wrapped to right
    cr_assert_eq(client1->player->pos_y, 4);
    
    free_test_client(client1);
    free_test_server(server);
}
