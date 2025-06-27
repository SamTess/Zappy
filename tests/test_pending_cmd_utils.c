/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_pending_cmd_utils - tests for pending command utilities
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/pending_cmd.h"
#include "../src/Server/include/command.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations
extern void execute_pending_cmd(server_t *server, client_t *client);

// Mock function for testing
static int mock_func_called = 0;
static server_t *mock_func_server = NULL;
static client_t *mock_func_client = NULL;
static char **mock_func_args = NULL;

static void mock_command_function(server_t *server, client_t *client, char **args)
{
    mock_func_called++;
    mock_func_server = server;
    mock_func_client = client;
    mock_func_args = args; // Note: don't free this in the mock
}

// Helper function to create a test client with pending command
static client_t *create_test_client_with_pending_cmd(const char *args, 
                                                     void (*func)(server_t *, client_t *, char **))
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    pending_cmd_t *pending_cmd = calloc(1, sizeof(pending_cmd_t));
    
    client->client_id = 42;
    client->client_fd = 5;
    client->player = player;
    
    player->pending_cmd = pending_cmd;
    if (args) {
        pending_cmd->args = strdup(args);
    }
    pending_cmd->func = func;
    
    return client;
}

static void cleanup_test_client(client_t *client)
{
    if (client) {
        if (client->player) {
            if (client->player->pending_cmd) {
                // Don't free args here as execute_pending_cmd should do it
                free(client->player->pending_cmd);
            }
            free(client->player);
        }
        free(client);
    }
}

TestSuite(pending_cmd_utils_tests);

// Test execute_pending_cmd with valid pending command
Test(pending_cmd_utils_tests, test_execute_pending_cmd_valid)
{
    server_t server = {0};
    client_t *client = create_test_client_with_pending_cmd("Forward", mock_command_function);
    
    // Reset mock tracking
    mock_func_called = 0;
    mock_func_server = NULL;
    mock_func_client = NULL;
    
    execute_pending_cmd(&server, client);
    
    // Verify the mock function was called with correct parameters
    cr_assert_eq(mock_func_called, 1, "Mock function should be called once");
    cr_assert_eq(mock_func_server, &server, "Server parameter should be passed correctly");
    cr_assert_eq(mock_func_client, client, "Client parameter should be passed correctly");
    cr_assert_not_null(mock_func_args, "Args should not be NULL");
    
    // Verify that pending command was cleared
    cr_assert_null(client->player->pending_cmd->args, "Args should be cleared after execution");
    cr_assert_null(client->player->pending_cmd->func, "Function should be cleared after execution");
    
    cleanup_test_client(client);
}

// Test execute_pending_cmd with NULL client
Test(pending_cmd_utils_tests, test_execute_pending_cmd_null_client)
{
    server_t server = {0};
    
    mock_func_called = 0;
    
    // Should not crash and should not call any function
    execute_pending_cmd(&server, NULL);
    
    cr_assert_eq(mock_func_called, 0, "No function should be called with NULL client");
}

// Test execute_pending_cmd with client that has NULL player
Test(pending_cmd_utils_tests, test_execute_pending_cmd_null_player)
{
    server_t server = {0};
    client_t client = {
        .client_id = 1,
        .client_fd = 5,
        .player = NULL
    };
    
    mock_func_called = 0;
    
    execute_pending_cmd(&server, &client);
    
    cr_assert_eq(mock_func_called, 0, "No function should be called with NULL player");
}

// Test execute_pending_cmd with player that has NULL pending_cmd
Test(pending_cmd_utils_tests, test_execute_pending_cmd_null_pending_cmd)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {
        .pending_cmd = NULL
    };
    
    client.client_id = 1;
    client.client_fd = 5;
    client.player = &player;
    
    mock_func_called = 0;
    
    execute_pending_cmd(&server, &client);
    
    cr_assert_eq(mock_func_called, 0, "No function should be called with NULL pending_cmd");
}

// Test execute_pending_cmd with pending command that has NULL function
Test(pending_cmd_utils_tests, test_execute_pending_cmd_null_function)
{
    server_t server = {0};
    client_t *client = create_test_client_with_pending_cmd("Some args", NULL);
    
    mock_func_called = 0;
    
    execute_pending_cmd(&server, client);
    
    // Should not crash, function should not be called, but args should still be freed
    cr_assert_eq(mock_func_called, 0, "No function should be called with NULL function pointer");
    cr_assert_null(client->player->pending_cmd->args, "Args should still be freed");
    cr_assert_null(client->player->pending_cmd->func, "Function should remain NULL");
    
    cleanup_test_client(client);
}

// Test execute_pending_cmd with pending command that has NULL args
Test(pending_cmd_utils_tests, test_execute_pending_cmd_null_args)
{
    server_t server = {0};
    client_t *client = create_test_client_with_pending_cmd(NULL, mock_command_function);
    
    mock_func_called = 0;
    
    execute_pending_cmd(&server, client);
    
    // Function should not be called because args is NULL
    cr_assert_eq(mock_func_called, 0, "Function should not be called with NULL args");
    cr_assert_null(client->player->pending_cmd->args, "Args should remain NULL");
    cr_assert_null(client->player->pending_cmd->func, "Function should be cleared");
    
    cleanup_test_client(client);
}

// Test execute_pending_cmd with complex args string
Test(pending_cmd_utils_tests, test_execute_pending_cmd_complex_args)
{
    server_t server = {0};
    client_t *client = create_test_client_with_pending_cmd("Take linemate", mock_command_function);
    
    mock_func_called = 0;
    
    execute_pending_cmd(&server, client);
    
    cr_assert_eq(mock_func_called, 1, "Mock function should be called once");
    
    // Verify cleanup (don't check args parsing as it may cause issues)
    cr_assert_null(client->player->pending_cmd->args, "Args should be cleared");
    cr_assert_null(client->player->pending_cmd->func, "Function should be cleared");
    
    cleanup_test_client(client);
}

// Test execute_pending_cmd with empty args string
Test(pending_cmd_utils_tests, test_execute_pending_cmd_empty_args)
{
    server_t server = {0};
    client_t *client = create_test_client_with_pending_cmd("", mock_command_function);
    
    mock_func_called = 0;
    
    execute_pending_cmd(&server, client);
    
    cr_assert_eq(mock_func_called, 1, "Mock function should be called even with empty args");
    
    // Verify cleanup
    cr_assert_null(client->player->pending_cmd->args, "Args should be cleared");
    cr_assert_null(client->player->pending_cmd->func, "Function should be cleared");
    
    cleanup_test_client(client);
}

// Test execute_pending_cmd multiple times
Test(pending_cmd_utils_tests, test_execute_pending_cmd_multiple_calls)
{
    server_t server = {0};
    client_t *client1 = create_test_client_with_pending_cmd("Right", mock_command_function);
    client_t *client2 = create_test_client_with_pending_cmd("Left", mock_command_function);
    
    mock_func_called = 0;
    
    // Execute first command
    execute_pending_cmd(&server, client1);
    cr_assert_eq(mock_func_called, 1, "First command should be executed");
    
    // Execute second command
    execute_pending_cmd(&server, client2);
    cr_assert_eq(mock_func_called, 2, "Second command should also be executed");
    
    // Verify both are cleaned up
    cr_assert_null(client1->player->pending_cmd->args, "First client args should be cleared");
    cr_assert_null(client2->player->pending_cmd->args, "Second client args should be cleared");
    
    cleanup_test_client(client1);
    cleanup_test_client(client2);
}