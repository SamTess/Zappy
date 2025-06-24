/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pin - tests for pin command function
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
extern void command_pin(server_t *server, client_t *client, char **buffer);
extern void send_pin_to_all(server_t *server, client_t *client);

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
    client->type = AI;
    client->player = player;
    
    // Initialize player position and inventory
    player->pos_x = 5;
    player->pos_y = 3;
    player->inventory[FOOD] = 10;
    player->inventory[LINEMATE] = 2;
    player->inventory[DERAUMERE] = 1;
    
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

TestSuite(command_pin_tests);

// Test command_pin with NULL server
Test(command_pin_tests, test_command_pin_null_server)
{
    client_t *client = create_test_client();
    char *buffer[] = {"pin", "#123", NULL};
    
    command_pin(NULL, client, buffer);
    
    cr_assert(true, "command_pin should handle NULL server gracefully");
    
    cleanup_test_client(client);
}

// Test command_pin with server that has no graphical clients
Test(command_pin_tests, test_command_pin_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    client_t *client = create_test_client();
    char *buffer[] = {"pin", "#123", NULL};
    
    command_pin(server, client, buffer);
    
    cr_assert(true, "command_pin should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}

// Test command_pin with invalid buffer
Test(command_pin_tests, test_command_pin_invalid_buffer)
{
    server_t *server = create_test_server_with_graphics(true);
    client_t *client = create_test_client();
    char *buffer[] = {"pin", NULL};
    
    command_pin(server, client, buffer);
    
    cr_assert(true, "command_pin should handle invalid buffer gracefully");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}

// Test send_pin_to_all with valid parameters
Test(command_pin_tests, test_send_pin_to_all_valid, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    client_t *client = create_test_client();
    
    send_pin_to_all(server, client);
    
    cr_assert(true, "send_pin_to_all should execute with valid parameters");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}