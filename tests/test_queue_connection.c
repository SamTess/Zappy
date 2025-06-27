/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_queue_connection - tests for command queue management functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/player.h"
#include <stdlib.h>
#include <string.h>

// Test suite setup
TestSuite(queue_connection_tests);

// Mock variables
static int mock_execute_com_calls = 0;
static char *last_executed_command = NULL;
static server_t *last_execute_server = NULL;
static client_t *last_execute_client = NULL;

// Reset mock functions
static void reset_queue_mocks(void)
{
    mock_execute_com_calls = 0;
    if (last_executed_command) {
        free(last_executed_command);
        last_executed_command = NULL;
    }
    last_execute_server = NULL;
    last_execute_client = NULL;
}

// Mock implementation for execute_com
void __wrap_execute_com(server_t *server, client_t *client, char *command)
{
    mock_execute_com_calls++;
    last_execute_server = server;
    last_execute_client = client;
    if (last_executed_command) {
        free(last_executed_command);
    }
    last_executed_command = strdup(command);
}

// =============================================================================
// HELPER FUNCTIONS
// =============================================================================

/**
 * Create a test server with poll_manager
 */
static server_t *create_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    poll_manager_t *poll_manager = calloc(1, sizeof(poll_manager_t));
    
    server->poll_manager = poll_manager;
    poll_manager->needs_rebuild = false;
    
    return server;
}

/**
 * Create a test client with player and command queue
 */
static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->player = player;
    player->command_queue = calloc(10, sizeof(char *));
    player->queue_size = 0;
    
    return client;
}

/**
 * Free test server
 */
static void free_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->poll_manager) {
        free(server->poll_manager);
    }
    free(server);
}

/**
 * Free test client
 */
static void free_test_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        if (client->player->command_queue) {
            for (int i = 0; i < 10; i++) {
                if (client->player->command_queue[i]) {
                    free(client->player->command_queue[i]);
                }
            }
            free(client->player->command_queue);
        }
        free(client->player);
    }
    free(client);
}

// =============================================================================
// ADD_TO_COMMAND_QUEUE TESTS
// =============================================================================

Test(queue_connection_tests, test_add_to_command_queue_null_client)
{
    server_t *server = create_test_server();
    
    reset_queue_mocks();
    
    // Should return early when client is NULL
    add_to_command_queue(server, NULL, "forward");
    
    // No side effects should occur
    cr_assert_eq(mock_execute_com_calls, 0);
    
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_null_player)
{
    server_t *server = create_test_server();
    client_t *client = calloc(1, sizeof(client_t));
    client->player = NULL;
    
    reset_queue_mocks();
    
    // Should return early when player is NULL
    add_to_command_queue(server, client, "forward");
    
    cr_assert_eq(mock_execute_com_calls, 0);
    
    free(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_null_command)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    reset_queue_mocks();
    
    // Should return early when command is NULL
    add_to_command_queue(server, client, NULL);
    
    cr_assert_eq(client->player->queue_size, 0);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_null_command_queue)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Set command_queue to NULL
    free(client->player->command_queue);
    client->player->command_queue = NULL;
    
    reset_queue_mocks();
    
    // Should return early when command_queue is NULL
    add_to_command_queue(server, client, "forward");
    
    cr_assert_eq(client->player->queue_size, 0);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_queue_full)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Fill the queue to capacity
    client->player->queue_size = 10;
    
    reset_queue_mocks();
    
    // Should return early when queue is full
    add_to_command_queue(server, client, "forward");
    
    cr_assert_eq(client->player->queue_size, 10);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_successful)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    reset_queue_mocks();
    
    // Should successfully add command to queue
    add_to_command_queue(server, client, "forward");
    
    cr_assert_eq(client->player->queue_size, 1);
    cr_assert_not_null(client->player->command_queue[0]);
    cr_assert_str_eq(client->player->command_queue[0], "forward");
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_multiple_commands)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    reset_queue_mocks();
    
    // Add multiple commands
    add_to_command_queue(server, client, "forward");
    add_to_command_queue(server, client, "left");
    add_to_command_queue(server, client, "right");
    
    cr_assert_eq(client->player->queue_size, 3);
    cr_assert_str_eq(client->player->command_queue[0], "forward");
    cr_assert_str_eq(client->player->command_queue[1], "left");
    cr_assert_str_eq(client->player->command_queue[2], "right");
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_triggers_rebuild)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Fill queue to 9 commands
    for (int i = 0; i < 9; i++) {
        client->player->command_queue[i] = strdup("test");
        client->player->queue_size++;
    }
    
    reset_queue_mocks();
    
    // Adding 10th command should trigger rebuild
    add_to_command_queue(server, client, "forward");
    
    cr_assert_eq(client->player->queue_size, 10);
    cr_assert(server->poll_manager->needs_rebuild);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_add_to_command_queue_no_server)
{
    client_t *client = create_test_client();
    
    reset_queue_mocks();
    
    // Should work even without server
    add_to_command_queue(NULL, client, "forward");
    
    cr_assert_eq(client->player->queue_size, 1);
    cr_assert_str_eq(client->player->command_queue[0], "forward");
    
    free_test_client(client);
}

Test(queue_connection_tests, test_add_to_command_queue_no_poll_manager)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Remove poll_manager
    free(server->poll_manager);
    server->poll_manager = NULL;
    
    // Fill queue to 9 commands
    for (int i = 0; i < 9; i++) {
        client->player->command_queue[i] = strdup("test");
        client->player->queue_size++;
    }
    
    reset_queue_mocks();
    
    // Should not crash when poll_manager is NULL
    add_to_command_queue(server, client, "forward");
    
    cr_assert_eq(client->player->queue_size, 10);
    
    free_test_client(client);
    free_test_server(server);
}

// =============================================================================
// PROCESS_NEXT_QUEUED_COMMAND TESTS
// =============================================================================

Test(queue_connection_tests, test_process_next_queued_command_null_client)
{
    server_t *server = create_test_server();
    
    reset_queue_mocks();
    
    // Should return early when client is NULL
    process_next_queued_command(server, NULL);
    
    cr_assert_eq(mock_execute_com_calls, 0);
    
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_null_player)
{
    server_t *server = create_test_server();
    client_t *client = calloc(1, sizeof(client_t));
    client->player = NULL;
    
    reset_queue_mocks();
    
    // Should return early when player is NULL
    process_next_queued_command(server, client);
    
    cr_assert_eq(mock_execute_com_calls, 0);
    
    free(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_null_server)
{
    client_t *client = create_test_client();
    
    reset_queue_mocks();
    
    // Should return early when server is NULL
    process_next_queued_command(NULL, client);
    
    cr_assert_eq(mock_execute_com_calls, 0);
    
    free_test_client(client);
}

Test(queue_connection_tests, test_process_next_queued_command_empty_queue)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Queue is already empty (queue_size = 0)
    reset_queue_mocks();
    
    // Should return early when queue is empty
    process_next_queued_command(server, client);
    
    cr_assert_eq(mock_execute_com_calls, 0);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_null_command_queue)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Set command_queue to NULL but queue_size > 0
    free(client->player->command_queue);
    client->player->command_queue = NULL;
    client->player->queue_size = 1;
    
    reset_queue_mocks();
    
    // Should return early when command_queue is NULL
    process_next_queued_command(server, client);
    
    cr_assert_eq(mock_execute_com_calls, 0);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_single_command)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Add one command to queue
    client->player->command_queue[0] = strdup("forward");
    client->player->queue_size = 1;
    
    reset_queue_mocks();
    
    // Should process the command
    process_next_queued_command(server, client);
    
    cr_assert_eq(client->player->queue_size, 0);
    cr_assert_eq(mock_execute_com_calls, 1);
    cr_assert_str_eq(last_executed_command, "forward");
    cr_assert_eq(last_execute_server, server);
    cr_assert_eq(last_execute_client, client);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_multiple_commands)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Add multiple commands to queue
    client->player->command_queue[0] = strdup("forward");
    client->player->command_queue[1] = strdup("left");
    client->player->command_queue[2] = strdup("right");
    client->player->queue_size = 3;
    
    reset_queue_mocks();
    
    // Should process first command and shift others
    process_next_queued_command(server, client);
    
    cr_assert_eq(client->player->queue_size, 2);
    cr_assert_eq(mock_execute_com_calls, 1);
    cr_assert_str_eq(last_executed_command, "forward");
    
    // Check that commands were shifted correctly
    cr_assert_str_eq(client->player->command_queue[0], "left");
    cr_assert_str_eq(client->player->command_queue[1], "right");
    cr_assert_null(client->player->command_queue[2]);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_was_full_triggers_rebuild)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Fill queue to capacity
    for (int i = 0; i < 10; i++) {
        client->player->command_queue[i] = strdup("test");
    }
    client->player->queue_size = 10;
    
    reset_queue_mocks();
    
    // Process one command from full queue
    process_next_queued_command(server, client);
    
    cr_assert_eq(client->player->queue_size, 9);
    cr_assert(server->poll_manager->needs_rebuild);
    cr_assert_eq(mock_execute_com_calls, 1);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_not_full_no_rebuild)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Add only a few commands (not full)
    client->player->command_queue[0] = strdup("forward");
    client->player->command_queue[1] = strdup("left");
    client->player->queue_size = 2;
    
    reset_queue_mocks();
    
    // Process one command from non-full queue
    process_next_queued_command(server, client);
    
    cr_assert_eq(client->player->queue_size, 1);
    cr_assert_not(server->poll_manager->needs_rebuild);
    cr_assert_eq(mock_execute_com_calls, 1);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_null_command_in_queue)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Add NULL command to queue (edge case)
    client->player->command_queue[0] = NULL;
    client->player->queue_size = 1;
    
    reset_queue_mocks();
    
    // Should handle NULL command gracefully
    process_next_queued_command(server, client);
    
    cr_assert_eq(client->player->queue_size, 0);
    cr_assert_eq(mock_execute_com_calls, 0); // Should not call execute_com with NULL
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_process_next_queued_command_no_poll_manager)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Remove poll_manager
    free(server->poll_manager);
    server->poll_manager = NULL;
    
    // Fill queue to capacity
    for (int i = 0; i < 10; i++) {
        client->player->command_queue[i] = strdup("test");
    }
    client->player->queue_size = 10;
    
    reset_queue_mocks();
    
    // Should not crash when poll_manager is NULL
    process_next_queued_command(server, client);
    
    cr_assert_eq(client->player->queue_size, 9);
    cr_assert_eq(mock_execute_com_calls, 1);
    
    free_test_client(client);
    free_test_server(server);
}

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

Test(queue_connection_tests, test_full_queue_workflow)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    reset_queue_mocks();
    
    // Add commands until queue is full
    for (int i = 0; i < 10; i++) {
        char command[20];
        sprintf(command, "command_%d", i);
        add_to_command_queue(server, client, command);
    }
    
    cr_assert_eq(client->player->queue_size, 10);
    cr_assert(server->poll_manager->needs_rebuild);
    
    // Try to add one more (should be rejected)
    server->poll_manager->needs_rebuild = false;
    add_to_command_queue(server, client, "extra_command");
    
    cr_assert_eq(client->player->queue_size, 10);
    cr_assert_not(server->poll_manager->needs_rebuild);
    
    // Process all commands
    for (int i = 0; i < 10; i++) {
        process_next_queued_command(server, client);
    }
    
    cr_assert_eq(client->player->queue_size, 0);
    cr_assert_eq(mock_execute_com_calls, 10);
    
    free_test_client(client);
    free_test_server(server);
}

Test(queue_connection_tests, test_queue_shifting_correctness)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    // Add specific commands in order
    add_to_command_queue(server, client, "first");
    add_to_command_queue(server, client, "second");
    add_to_command_queue(server, client, "third");
    add_to_command_queue(server, client, "fourth");
    
    reset_queue_mocks();
    
    // Process first command
    process_next_queued_command(server, client);
    cr_assert_str_eq(last_executed_command, "first");
    cr_assert_str_eq(client->player->command_queue[0], "second");
    
    // Process second command
    process_next_queued_command(server, client);
    cr_assert_str_eq(last_executed_command, "second");
    cr_assert_str_eq(client->player->command_queue[0], "third");
    
    // Process third command
    process_next_queued_command(server, client);
    cr_assert_str_eq(last_executed_command, "third");
    cr_assert_str_eq(client->player->command_queue[0], "fourth");
    
    // Process last command
    process_next_queued_command(server, client);
    cr_assert_str_eq(last_executed_command, "fourth");
    cr_assert_eq(client->player->queue_size, 0);
    
    cr_assert_eq(mock_execute_com_calls, 4);
    
    free_test_client(client);
    free_test_server(server);
}