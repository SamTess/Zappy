/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Comprehensive tests for movement commands (forward, left, right)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "test_mocks.h"

// External mock variables from test_mocks.c
extern int mock_write_calls;
extern int mock_send_ppo_calls;
extern int mock_tile_add_calls;
extern int mock_tile_remove_calls;
extern char *last_message;

// Create test server with basic configuration
static server_t *create_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->parsed_info = calloc(1, sizeof(args_t));
    server->parsed_info->width = 10;
    server->parsed_info->height = 10;
    
    // Initialize map
    server->map = calloc(server->parsed_info->height, sizeof(tile_t *));
    for (int y = 0; y < server->parsed_info->height; y++) {
        server->map[y] = calloc(server->parsed_info->width, sizeof(tile_t));
    }
    
    return server;
}

// Create test client with player
static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->player = calloc(1, sizeof(player_t));
    client->client_fd = 1;
    client->type = AI;
    
    // Initialize player position and rotation
    client->player->pos_x = 5;
    client->player->pos_y = 5;
    client->player->rotation = UP;
    client->player->level = 1;
    
    return client;
}

// Clean up test data
static void cleanup_test_data(server_t *server, client_t *client)
{
    if (server) {
        if (server->map) {
            for (int y = 0; y < server->parsed_info->height; y++) {
                free(server->map[y]);
            }
            free(server->map);
        }
        if (server->parsed_info) free(server->parsed_info);
        free(server);
    }
    
    if (client) {
        if (client->player) free(client->player);
        free(client);
    }
    
    // Reset mock counters
    mock_write_calls = 0;
    mock_send_ppo_calls = 0;
    mock_tile_add_calls = 0;
    mock_tile_remove_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// FORWARD COMMAND TESTS
Test(movement_commands, forward_basic_movement_up)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Forward", NULL};
    
    client->player->rotation = UP;
    client->player->pos_x = 5;
    client->player->pos_y = 5;
    
    __wrap_forward(server, client, args);
    
    // Should move up (y decreases with wrapping)
    cr_assert_eq(client->player->pos_y, 4);
    cr_assert_eq(client->player->pos_x, 5); // x unchanged
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, forward_basic_movement_down)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Forward", NULL};
    
    client->player->rotation = DOWN;
    client->player->pos_x = 5;
    client->player->pos_y = 5;
    
    __wrap_forward(server, client, args);
    
    // Should move down (y increases with wrapping)
    cr_assert_eq(client->player->pos_y, 6);
    cr_assert_eq(client->player->pos_x, 5); // x unchanged
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, forward_basic_movement_left)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Forward", NULL};
    
    client->player->rotation = LEFT;
    client->player->pos_x = 5;
    client->player->pos_y = 5;
    
    __wrap_forward(server, client, args);
    
    // Should move left (x decreases with wrapping)
    cr_assert_eq(client->player->pos_x, 4);
    cr_assert_eq(client->player->pos_y, 5); // y unchanged
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, forward_basic_movement_right)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Forward", NULL};
    
    client->player->rotation = RIGHT;
    client->player->pos_x = 5;
    client->player->pos_y = 5;
    
    __wrap_forward(server, client, args);
    
    // Should move right (x increases with wrapping)
    cr_assert_eq(client->player->pos_x, 6);
    cr_assert_eq(client->player->pos_y, 5); // y unchanged
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, forward_wrapping_boundaries)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Forward", NULL};
    
    // Test wrapping at top boundary (UP)
    client->player->rotation = UP;
    client->player->pos_x = 5;
    client->player->pos_y = 0;
    
    __wrap_forward(server, client, args);
    
    // Should wrap to bottom
    cr_assert_eq(client->player->pos_y, 9);
    cr_assert_str_eq(last_message, "ok\n");
    
    // Test wrapping at bottom boundary (DOWN)
    client->player->rotation = DOWN;
    client->player->pos_y = 9;
    
    __wrap_forward(server, client, args);
    
    // Should wrap to top
    cr_assert_eq(client->player->pos_y, 0);
    
    cleanup_test_data(server, client);
}

Test(movement_commands, forward_null_client)
{
    server_t *server = create_test_server();
    char *args[] = {"Forward", NULL};
    
    __wrap_forward(server, NULL, args);
    
    // Should not crash and not write anything
    cr_assert_eq(mock_write_calls, 0);
    
    cleanup_test_data(server, NULL);
}

Test(movement_commands, forward_null_player)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Forward", NULL};
    
    free(client->player);
    client->player = NULL;
    
    __wrap_forward(server, client, args);
    
    cr_assert_str_eq(last_message, "ko\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, forward_invalid_args)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Forward", "extra", NULL}; // Too many args
    
    __wrap_forward(server, client, args);
    
    cr_assert_str_eq(last_message, "ko\n");
    
    cleanup_test_data(server, client);
}

// LEFT COMMAND TESTS
Test(movement_commands, left_rotation_from_up)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Left", NULL};
    
    client->player->rotation = UP;
    
    __wrap_left(server, client, args);
    
    cr_assert_eq(client->player->rotation, LEFT);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, left_rotation_from_left)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Left", NULL};
    
    client->player->rotation = LEFT;
    
    __wrap_left(server, client, args);
    
    cr_assert_eq(client->player->rotation, DOWN);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, left_rotation_from_down)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Left", NULL};
    
    client->player->rotation = DOWN;
    
    __wrap_left(server, client, args);
    
    cr_assert_eq(client->player->rotation, RIGHT);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, left_rotation_from_right)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Left", NULL};
    
    client->player->rotation = RIGHT;
    
    __wrap_left(server, client, args);
    
    cr_assert_eq(client->player->rotation, UP);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

// RIGHT COMMAND TESTS
Test(movement_commands, right_rotation_from_up)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Right", NULL};
    
    client->player->rotation = UP;
    
    __wrap_right(server, client, args);
    
    cr_assert_eq(client->player->rotation, RIGHT);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, right_rotation_from_right)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Right", NULL};
    
    client->player->rotation = RIGHT;
    
    __wrap_right(server, client, args);
    
    cr_assert_eq(client->player->rotation, DOWN);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, right_rotation_from_down)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Right", NULL};
    
    client->player->rotation = DOWN;
    
    __wrap_right(server, client, args);
    
    cr_assert_eq(client->player->rotation, LEFT);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, right_rotation_from_left)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Right", NULL};
    
    client->player->rotation = LEFT;
    
    __wrap_right(server, client, args);
    
    cr_assert_eq(client->player->rotation, UP);
    cr_assert_str_eq(last_message, "ok\n");
    
    cleanup_test_data(server, client);
}

// ERROR HANDLING TESTS
Test(movement_commands, left_null_client)
{
    server_t *server = create_test_server();
    char *args[] = {"Left", NULL};
    
    __wrap_left(server, NULL, args);
    
    // Should not crash
    cr_assert_eq(mock_write_calls, 0);
    
    cleanup_test_data(server, NULL);
}

Test(movement_commands, right_null_player)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *args[] = {"Right", NULL};
    
    free(client->player);
    client->player = NULL;
    
    __wrap_right(server, client, args);
    
    cr_assert_str_eq(last_message, "ko\n");
    
    cleanup_test_data(server, client);
}

Test(movement_commands, left_invalid_args)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Test NULL args
    __wrap_left(server, client, NULL);
    cr_assert_str_eq(last_message, "ko\n");
    
    // Test too many args
    char *args[] = {"Left", "extra", NULL};
    __wrap_left(server, client, args);
    cr_assert_str_eq(last_message, "ko\n");
    
    cleanup_test_data(server, client);
}

// INTEGRATION TESTS
Test(movement_commands, movement_sequence)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    char *forward_args[] = {"Forward", NULL};
    char *left_args[] = {"Left", NULL};
    char *right_args[] = {"Right", NULL};
    
    // Start at position (5,5) facing UP
    client->player->pos_x = 5;
    client->player->pos_y = 5;
    client->player->rotation = UP;
    
    // Move forward (should go to 5,4)
    __wrap_forward(server, client, forward_args);
    cr_assert_eq(client->player->pos_x, 5);
    cr_assert_eq(client->player->pos_y, 4);
    cr_assert_eq(client->player->rotation, UP);
    
    // Turn right (should face RIGHT)
    __wrap_right(server, client, right_args);
    cr_assert_eq(client->player->rotation, RIGHT);
    
    // Move forward (should go to 6,4)
    __wrap_forward(server, client, forward_args);
    cr_assert_eq(client->player->pos_x, 6);
    cr_assert_eq(client->player->pos_y, 4);
    
    // Turn left twice (should face LEFT)
    __wrap_left(server, client, left_args);
    cr_assert_eq(client->player->rotation, UP);
    __wrap_left(server, client, left_args);
    cr_assert_eq(client->player->rotation, LEFT);
    
    // Move forward (should go to 5,4)
    __wrap_forward(server, client, forward_args);
    cr_assert_eq(client->player->pos_x, 5);
    cr_assert_eq(client->player->pos_y, 4);
    
    cleanup_test_data(server, client);
}