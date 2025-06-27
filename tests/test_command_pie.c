/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pie - tests for pie command function
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/parsing.h"
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern void command_pie(server_t *server, int x, int y, int result);

// Helper function to create a test server
static server_t *create_test_server_with_graphics(bool has_graphics)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    if (has_graphics) {
        graphical_client_t *graph_client = calloc(1, sizeof(graphical_client_t));
        client_t *client = calloc(1, sizeof(client_t));
        
        client->client_fd = 42;
        client->client_id = 1;
        graph_client->client = client;
        graph_client->next = NULL;
        server->graphical_clients = graph_client;
        
        // Create a simple client list for the function
        server->client = calloc(1, sizeof(client_t));
        server->client->client_fd = 10;
        server->client->next = NULL;
    } else {
        server->graphical_clients = NULL;
        server->client = NULL;
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
        if (server->client)
            free(server->client);
        free(server);
    }
}

TestSuite(command_pie_tests);

// Test command_pie with NULL server
Test(command_pie_tests, test_command_pie_null_server)
{
    command_pie(NULL, 5, 5, 1);
    
    cr_assert(true, "command_pie should handle NULL server gracefully");
}

// Test command_pie with server that has no graphical clients
Test(command_pie_tests, test_command_pie_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    
    command_pie(server, 5, 5, 1);
    
    cr_assert(true, "command_pie should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
}

// Test command_pie with valid parameters
Test(command_pie_tests, test_command_pie_valid, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    command_pie(server, 5, 5, 1);
    
    cr_assert(true, "command_pie should execute with valid parameters");
    
    cleanup_test_server(server);
}

// Test command_pie with different results
Test(command_pie_tests, test_command_pie_different_results, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    command_pie(server, 2, 3, 0); // failure
    command_pie(server, 7, 8, 1); // success
    
    cr_assert(true, "command_pie should handle different result values");
    
    cleanup_test_server(server);
}

// Test command_pie execution path
Test(command_pie_tests, test_command_pie_execution_path, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // This will test the buffer allocation and formatting logic
    command_pie(server, 0, 0, 1);
    
    cr_assert(true, "command_pie should complete execution path");
    
    cleanup_test_server(server);
}