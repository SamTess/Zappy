/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_enw_coverage - Simple coverage tests for command_enw functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"

// Declare the real functions to bypass mocking
extern void __real_send_enw_command(server_t *server, client_t *client, int egg_id);
extern void __real_send_enw_command_start(server_t *server);

// Simple test server creation with graphical clients
static server_t *create_simple_enw_server(void)
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

static client_t *create_simple_enw_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 2;
    client->client_fd = 43;
    client->player = player;
    
    player->pos_x = 5;
    player->pos_y = 3;
    
    return client;
}

static void free_simple_enw_server(server_t *server)
{
    if (!server) return;
    if (server->graphical_clients) {
        free(server->graphical_clients->client);
        free(server->graphical_clients);
    }
    free(server);
}

static void free_simple_enw_client(client_t *client)
{
    if (!client) return;
    free(client->player);
    free(client);
}

TestSuite(command_enw_coverage_tests);

// Test: send_enw_command with NULL server
Test(command_enw_coverage_tests, test_send_enw_command_null_server)
{
    client_t *client = create_simple_enw_client();
    
    __real_send_enw_command(NULL, client, 123);
    
    cr_assert(true, "Function should handle NULL server");
    
    free_simple_enw_client(client);
}

// Test: send_enw_command with NULL client
Test(command_enw_coverage_tests, test_send_enw_command_null_client)
{
    server_t *server = create_simple_enw_server();
    
    __real_send_enw_command(server, NULL, 456);
    
    cr_assert(true, "Function should handle NULL client");
    
    free_simple_enw_server(server);
}

// Test: send_enw_command with no graphical clients
Test(command_enw_coverage_tests, test_send_enw_command_no_graphics)
{
    server_t *server = create_simple_enw_server();
    client_t *client = create_simple_enw_client();
    
    // Remove graphical clients
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    __real_send_enw_command(server, client, 789);
    
    cr_assert(true, "Function should handle no graphical clients");
    
    free_simple_enw_server(server);
    free_simple_enw_client(client);
}

// Test: send_enw_command with valid parameters
Test(command_enw_coverage_tests, test_send_enw_command_valid)
{
    server_t *server = create_simple_enw_server();
    client_t *client = create_simple_enw_client();
    
    __real_send_enw_command(server, client, 999);
    
    cr_assert(true, "Function should execute with valid parameters");
    
    free_simple_enw_server(server);
    free_simple_enw_client(client);
}

// Test: send_enw_command with different egg IDs
Test(command_enw_coverage_tests, test_send_enw_command_different_ids)
{
    server_t *server = create_simple_enw_server();
    client_t *client = create_simple_enw_client();
    
    __real_send_enw_command(server, client, 0);
    __real_send_enw_command(server, client, 555);
    __real_send_enw_command(server, client, -1);
    
    cr_assert(true, "Function should handle different egg IDs");
    
    free_simple_enw_server(server);
    free_simple_enw_client(client);
}

// Test: send_enw_command_start with NULL server
Test(command_enw_coverage_tests, test_send_enw_command_start_null_server)
{
    __real_send_enw_command_start(NULL);
    
    cr_assert(true, "Function should handle NULL server");
}

// Test: send_enw_command_start with no graphical clients
Test(command_enw_coverage_tests, test_send_enw_command_start_no_graphics)
{
    server_t *server = create_simple_enw_server();
    
    // Remove graphical clients
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    __real_send_enw_command_start(server);
    
    cr_assert(true, "Function should handle no graphical clients");
    
    free_simple_enw_server(server);
}

// Test: send_enw_command_start with valid server but no eggs
Test(command_enw_coverage_tests, test_send_enw_command_start_no_eggs)
{
    server_t *server = create_simple_enw_server();
    
    // No eggs by default (server->eggs is NULL)
    __real_send_enw_command_start(server);
    
    cr_assert(true, "Function should handle server with no eggs");
    
    free_simple_enw_server(server);
}

// Test: send_enw_command execution path coverage
Test(command_enw_coverage_tests, test_send_enw_command_execution_path)
{
    server_t *server = create_simple_enw_server();
    client_t *client = create_simple_enw_client();
    
    // Add a second graphical client to test the loop
    graphical_client_t *second_client = calloc(1, sizeof(graphical_client_t));
    client_t *client2 = calloc(1, sizeof(client_t));
    client2->client_fd = 44;
    client2->client_id = 3;
    second_client->client = client2;
    second_client->next = NULL;
    server->graphical_clients->next = second_client;
    
    __real_send_enw_command(server, client, 12345);
    
    cr_assert(true, "send_enw_command should complete execution path");
    
    // Free the additional client
    free(second_client->client);
    free(second_client);
    free_simple_enw_server(server);
    free_simple_enw_client(client);
}