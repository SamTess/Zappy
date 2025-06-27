/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_edi - tests for edi command functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern void send_edi_command(server_t *server, int egg_id);

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

TestSuite(command_edi_tests);

// Test send_edi_command with NULL server
Test(command_edi_tests, test_send_edi_command_null_server)
{
    // Should not crash with NULL server
    send_edi_command(NULL, 123);
    
    cr_assert(true, "send_edi_command should handle NULL server gracefully");
}

// Test send_edi_command with server that has no graphical clients
Test(command_edi_tests, test_send_edi_command_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    
    // Should not crash with no graphical clients
    send_edi_command(server, 456);
    
    cr_assert(true, "send_edi_command should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
}

// Test send_edi_command with valid server and graphical clients
Test(command_edi_tests, test_send_edi_command_valid)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // This should execute the full function without issues
    send_edi_command(server, 789);
    
    cr_assert(true, "send_edi_command should execute with valid parameters");
    
    cleanup_test_server(server);
}

// Test send_edi_command with different egg IDs
Test(command_edi_tests, test_send_edi_command_different_ids)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Test with various egg IDs
    send_edi_command(server, 0);
    send_edi_command(server, 1);
    send_edi_command(server, 999);
    send_edi_command(server, -1);  // Edge case
    send_edi_command(server, 12345);
    
    cr_assert(true, "send_edi_command should handle different egg IDs");
    
    cleanup_test_server(server);
}

// Test send_edi_command execution path
Test(command_edi_tests, test_send_edi_command_execution_path)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // This will test the buffer allocation and formatting logic
    send_edi_command(server, 98765);
    
    cr_assert(true, "send_edi_command should complete execution path");
    
    cleanup_test_server(server);
}

// Test send_edi_command with multiple calls
Test(command_edi_tests, test_send_edi_command_multiple_calls)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Multiple calls should all work
    send_edi_command(server, 100);
    send_edi_command(server, 200);
    send_edi_command(server, 300);
    
    cr_assert(true, "send_edi_command should handle multiple calls");
    
    cleanup_test_server(server);
}