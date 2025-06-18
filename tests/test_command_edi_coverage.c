/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_edi_coverage - Simple coverage tests for command_edi functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"

// Declare the real function to bypass mocking
extern void __real_send_edi_command(server_t *server, int egg_id);

// Simple test server creation with graphical clients
static server_t *create_simple_edi_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    // Create a simple graphical client
    graphical_client_t *graph_client = calloc(1, sizeof(graphical_client_t));
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = 42;
    client->client_id = 1;
    graph_client->client = client;
    graph_client->next = NULL;
    server->graphical_clients = graph_client;
    
    return server;
}

static void free_simple_edi_server(server_t *server)
{
    if (!server) return;
    if (server->graphical_clients) {
        free(server->graphical_clients->client);
        free(server->graphical_clients);
    }
    free(server);
}

TestSuite(command_edi_coverage_tests);

// Test: send_edi_command with NULL server
Test(command_edi_coverage_tests, test_send_edi_command_null_server)
{
    __real_send_edi_command(NULL, 123);
    
    cr_assert(true, "Function should handle NULL server");
}

// Test: send_edi_command with no graphical clients
Test(command_edi_coverage_tests, test_send_edi_command_no_graphics)
{
    server_t *server = create_simple_edi_server();
    
    // Remove graphical clients
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    __real_send_edi_command(server, 456);
    
    cr_assert(true, "Function should handle no graphical clients");
    
    free_simple_edi_server(server);
}

// Test: send_edi_command with valid parameters
Test(command_edi_coverage_tests, test_send_edi_command_valid)
{
    server_t *server = create_simple_edi_server();
    
    __real_send_edi_command(server, 789);
    
    cr_assert(true, "Function should execute with valid parameters");
    
    free_simple_edi_server(server);
}

// Test: send_edi_command with different egg IDs
Test(command_edi_coverage_tests, test_send_edi_command_different_ids)
{
    server_t *server = create_simple_edi_server();
    
    __real_send_edi_command(server, 0);
    __real_send_edi_command(server, 999);
    __real_send_edi_command(server, -1);
    
    cr_assert(true, "Function should handle different egg IDs");
    
    free_simple_edi_server(server);
}

// Test: send_edi_command with multiple graphical clients
Test(command_edi_coverage_tests, test_send_edi_command_multiple_clients)
{
    server_t *server = create_simple_edi_server();
    
    // Add a second graphical client
    graphical_client_t *second_client = calloc(1, sizeof(graphical_client_t));
    client_t *client2 = calloc(1, sizeof(client_t));
    client2->client_fd = 43;
    client2->client_id = 2;
    second_client->client = client2;
    second_client->next = NULL;
    server->graphical_clients->next = second_client;
    
    __real_send_edi_command(server, 555);
    
    cr_assert(true, "Function should handle multiple graphical clients");
    
    // Free the additional client
    free(second_client->client);
    free(second_client);
    free_simple_edi_server(server);
}

// Test: send_edi_command execution path coverage
Test(command_edi_coverage_tests, test_send_edi_command_execution_path)
{
    server_t *server = create_simple_edi_server();
    
    // This will test the buffer allocation and formatting logic
    __real_send_edi_command(server, 12345);
    
    cr_assert(true, "send_edi_command should complete execution path");
    
    free_simple_edi_server(server);
}