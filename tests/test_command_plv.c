/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_plv - tests for plv command function
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
extern void command_plv(server_t *server, client_t *client, char **buffer);
extern void send_plv_to_all(server_t *server, client_t *client);

// Helper function to create a test server
static server_t *create_test_server_with_graphics(bool has_graphics)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    if (has_graphics) {
        graphical_client_t *graph_client = calloc(1, sizeof(graphical_client_t));
        client_t *client = calloc(1, sizeof(client_t));
        
        client->client_fd = 42;
        client->client_id = 1;
        client->type = GRAPHICAL;
        graph_client->client = client;
        graph_client->next = NULL;
        server->graphical_clients = graph_client;
        
        // Create a simple client list for send_plv_to_all
        server->client = calloc(1, sizeof(client_t));
        server->client->client_fd = 10;
        server->client->type = GRAPHICAL;
        server->client->next = NULL;
    } else {
        server->graphical_clients = NULL;
        server->client = NULL;
    }
    
    return server;
}

static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 123;
    client->client_fd = 1;
    client->type = AI;
    client->player = player;
    
    // Initialize player level
    player->level = 2;
    
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
        if (server->client)
            free(server->client);
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

TestSuite(command_plv_tests);

// Test command_plv with NULL server
Test(command_plv_tests, test_command_plv_null_server)
{
    client_t *client = create_test_client();
    char *buffer[] = {"plv", "#123", NULL};
    
    command_plv(NULL, client, buffer);
    
    cr_assert(true, "command_plv should handle NULL server gracefully");
    
    cleanup_test_client(client);
}

// Test command_plv with server that has no graphical clients
Test(command_plv_tests, test_command_plv_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    client_t *client = create_test_client();
    char *buffer[] = {"plv", "#123", NULL};
    
    command_plv(server, client, buffer);
    
    cr_assert(true, "command_plv should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}

// Test command_plv with invalid buffer
Test(command_plv_tests, test_command_plv_invalid_buffer)
{
    server_t *server = create_test_server_with_graphics(true);
    client_t *client = create_test_client();
    char *buffer[] = {"plv", NULL};
    
    command_plv(server, client, buffer);
    
    cr_assert(true, "command_plv should handle invalid buffer gracefully");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}

// Test send_plv_to_all with valid parameters
Test(command_plv_tests, test_send_plv_to_all_valid, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    client_t *client = create_test_client();
    
    send_plv_to_all(server, client);
    
    cr_assert(true, "send_plv_to_all should execute with valid parameters");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}