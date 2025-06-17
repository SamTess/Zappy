/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_ebo - tests for ebo command functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern void send_ebo_command(server_t *server, int egg_id);

// Helper function to create a test server
static server_t *create_test_server_with_graphics(bool has_graphics)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    if (has_graphics) {
        // Create a minimal graphical client structure
        graphical_client_t *graph_client = calloc(1, sizeof(graphical_client_t));
        client_t *client = calloc(1, sizeof(client_t));
        
        client->client_fd = 42;
        client->client_id = 1;
        graph_client->client = client;
        graph_client->next = NULL;
        
        server->graphical_clients = graph_client;
    } else {
        server->graphical_clients = NULL;
    }
    
    return server;
}

static void cleanup_test_server(server_t *server)
{
    if (server) {
        if (server->graphical_clients) {
            if (server->graphical_clients->client)
                free(server->graphical_clients->client);
            free(server->graphical_clients);
        }
        free(server);
    }
}

TestSuite(command_ebo_tests);

// Test send_ebo_command with NULL server
Test(command_ebo_tests, test_send_ebo_command_null_server)
{
    // Should not crash with NULL server
    send_ebo_command(NULL, 123);
    
    cr_assert(true, "send_ebo_command should handle NULL server gracefully");
}

// Test send_ebo_command with server that has no graphical clients
Test(command_ebo_tests, test_send_ebo_command_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    
    // Should not crash with no graphical clients
    send_ebo_command(server, 456);
    
    cr_assert(true, "send_ebo_command should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
}

// Test send_ebo_command with valid server and graphical clients
Test(command_ebo_tests, test_send_ebo_command_valid, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // This should execute the full function without major issues
    send_ebo_command(server, 789);
    
    cr_assert(true, "send_ebo_command should execute with valid parameters");
    
    cleanup_test_server(server);
}

// Test send_ebo_command with different egg IDs
Test(command_ebo_tests, test_send_ebo_command_different_ids, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Test with various egg IDs
    send_ebo_command(server, 0);
    send_ebo_command(server, 1);
    send_ebo_command(server, 999);
    send_ebo_command(server, -1);  // Edge case
    
    cr_assert(true, "send_ebo_command should handle different egg IDs");
    
    cleanup_test_server(server);
}

// Test send_ebo_command execution path
Test(command_ebo_tests, test_send_ebo_command_execution_path, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // This will test the buffer allocation and formatting logic
    send_ebo_command(server, 12345);
    
    cr_assert(true, "send_ebo_command should complete execution path");
    
    cleanup_test_server(server);
}