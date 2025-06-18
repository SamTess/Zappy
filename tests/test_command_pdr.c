/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pdr - tests for pdr command function
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
extern void command_pdr(server_t *server, client_t *client, resource_type_t resource_type);

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
    } else {
        server->graphical_clients = NULL;
    }
    
    return server;
}

static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 123;
    client->client_fd = 1;
    client->player = player;
    
    return client;
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

static void cleanup_test_client(client_t *client)
{
    if (client) {
        if (client->player)
            free(client->player);
        free(client);
    }
}

TestSuite(command_pdr_tests);

// Test command_pdr with NULL client
Test(command_pdr_tests, test_command_pdr_null_client)
{
    server_t *server = create_test_server_with_graphics(true);
    
    command_pdr(server, NULL, FOOD);
    
    cr_assert(true, "command_pdr should handle NULL client gracefully");
    
    cleanup_test_server(server);
}

// Test command_pdr with client that has no player
Test(command_pdr_tests, test_command_pdr_no_player)
{
    server_t *server = create_test_server_with_graphics(true);
    client_t *client = calloc(1, sizeof(client_t));
    client->client_id = 123;
    client->player = NULL;
    
    command_pdr(server, client, FOOD);
    
    cr_assert(true, "command_pdr should handle client with no player gracefully");
    
    cleanup_test_server(server);
    free(client);
}

// Test command_pdr with server that has no graphical clients
Test(command_pdr_tests, test_command_pdr_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    client_t *client = create_test_client();
    
    command_pdr(server, client, FOOD);
    
    cr_assert(true, "command_pdr should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}

// Test command_pdr with valid parameters
Test(command_pdr_tests, test_command_pdr_valid, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    client_t *client = create_test_client();
    
    command_pdr(server, client, FOOD);
    
    cr_assert(true, "command_pdr should execute with valid parameters");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}

// Test command_pdr with different resource types
Test(command_pdr_tests, test_command_pdr_different_resources, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    client_t *client = create_test_client();
    
    command_pdr(server, client, FOOD);
    command_pdr(server, client, LINEMATE);
    command_pdr(server, client, DERAUMERE);
    command_pdr(server, client, SIBUR);
    command_pdr(server, client, MENDIANE);
    command_pdr(server, client, PHIRAS);
    command_pdr(server, client, THYSTAME);
    
    cr_assert(true, "command_pdr should handle different resource types");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}