/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_broadcast
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Include necessary headers first
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "../src/Server/include/parsing.h"

// Mock functions for dependencies
void mock_start_incantation(server_t *server, client_t *client, char **args)
{
    (void)server;
    (void)client;
    (void)args;
}

// Helper functions to create test structures
static client_t *create_test_client(int id, int x, int y, enum rotation_e rotation)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->client_id = id;
    client->client_fd = -1; // Mock fd
    client->player = calloc(1, sizeof(player_t));
    client->player->pos_x = x;
    client->player->pos_y = y;
    client->player->rotation = rotation;
    client->type = AI;
    return client;
}

static server_t *create_test_server(int width, int height)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->parsed_info = calloc(1, sizeof(parsing_info_t));
    server->parsed_info->width = width;
    server->parsed_info->height = height;
    return server;
}

static void free_test_client(client_t *client)
{
    if (client) {
        if (client->player)
            free(client->player);
        free(client);
    }
}

static void free_test_server(server_t *server)
{
    if (server) {
        if (server->parsed_info)
            free(server->parsed_info);
        free(server);
    }
}

// Test main broadcast function with basic functionality
Test(broadcast, broadcast_null_buffer)
{
    server_t *server = create_test_server(10, 10);
    client_t *user = create_test_client(1, 5, 5, UP);
    
    broadcast(server, user, NULL);
    // Should handle null buffer gracefully
    
    free_test_client(user);
    free_test_server(server);
}

Test(broadcast, broadcast_insufficient_args)
{
    server_t *server = create_test_server(10, 10);
    client_t *user = create_test_client(1, 5, 5, UP);
    char *buffer[] = {"broadcast", NULL};
    
    broadcast(server, user, buffer);
    // Should handle insufficient arguments gracefully
    
    free_test_client(user);
    free_test_server(server);
}

Test(broadcast, broadcast_empty_buffer)
{
    server_t *server = create_test_server(10, 10);
    client_t *user = create_test_client(1, 5, 5, UP);
    char *buffer[] = {NULL};
    
    broadcast(server, user, buffer);
    // Should handle empty buffer gracefully
    
    free_test_client(user);
    free_test_server(server);
}

Test(broadcast, broadcast_valid_case_no_other_clients)
{
    server_t *server = create_test_server(10, 10);
    client_t *user = create_test_client(1, 5, 5, UP);
    char *buffer[] = {"broadcast", "hello", "world", NULL};
    
    user->client_fd = 1; // Mock fd
    
    // Set up server client list with just the user
    server->client = user;
    user->next = NULL;
    
    broadcast(server, user, buffer);
    // Should send to graphical clients and respond with "ok"
    
    // Don't free user here since it's referenced by server
    server->client = NULL;
    free_test_client(user);
    free_test_server(server);
}

Test(broadcast, broadcast_valid_case_with_other_clients)
{
    server_t *server = create_test_server(10, 10);
    client_t *user = create_test_client(1, 5, 5, UP);
    client_t *other = create_test_client(2, 7, 3, RIGHT);
    client_t *graphical = create_test_client(3, 0, 0, UP);
    char *buffer[] = {"broadcast", "test", "message", NULL};
    
    user->client_fd = 1;
    other->client_fd = 2;
    graphical->client_fd = 3;
    graphical->type = GRAPHICAL;
    
    // Set up server client list
    server->client = user;
    user->next = other;
    other->next = graphical;
    graphical->next = NULL;
    
    broadcast(server, user, buffer);
    // Should send to other AI clients and respond with "ok"
    
    // Clean up
    server->client = NULL;
    free_test_client(user);
    free_test_client(other);
    free_test_client(graphical);
    free_test_server(server);
}

// Edge case tests for wrapping coordinates
Test(broadcast, broadcast_edge_coordinates_wrapping)
{
    server_t *server = create_test_server(5, 5);
    client_t *sender = create_test_client(1, 0, 0, UP);
    client_t *receiver = create_test_client(2, 4, 4, UP);
    char *buffer[] = {"broadcast", "edge", "test", NULL};
    
    sender->client_fd = 1;
    receiver->client_fd = 2;
    
    // Set up server client list
    server->client = sender;
    sender->next = receiver;
    receiver->next = NULL;
    
    broadcast(server, sender, buffer);
    // Should handle wrapping coordinates correctly
    
    // Clean up
    server->client = NULL;
    free_test_client(sender);
    free_test_client(receiver);
    free_test_server(server);
}

// Test with different rotation combinations
Test(broadcast, broadcast_all_rotations)
{
    server_t *server = create_test_server(10, 10);
    client_t *sender = create_test_client(1, 5, 5, UP);
    client_t *receivers[4];
    char *buffer[] = {"broadcast", "rotation", "test", NULL};
    
    sender->client_fd = 1;
    
    // Create receivers with different rotations
    for (int i = 0; i < 4; i++) {
        receivers[i] = create_test_client(i + 2, 7, 5, (enum rotation_e)(i + 1));
        receivers[i]->client_fd = i + 2;
    }
    
    // Set up server client list
    server->client = sender;
    client_t *current = sender;
    for (int i = 0; i < 4; i++) {
        current->next = receivers[i];
        current = receivers[i];
    }
    current->next = NULL;
    
    broadcast(server, sender, buffer);
    // Should send to all receivers with different rotation calculations
    
    // Clean up
    server->client = NULL;
    free_test_client(sender);
    for (int i = 0; i < 4; i++) {
        free_test_client(receivers[i]);
    }
    free_test_server(server);
}

// Test with null user player
Test(broadcast, broadcast_null_user_player)
{
    server_t *server = create_test_server(10, 10);
    client_t *user = create_test_client(1, 5, 5, UP);
    char *buffer[] = {"broadcast", "hello", NULL};
    
    free(user->player);
    user->player = NULL;
    
    broadcast(server, user, buffer);
    // Should handle user without player gracefully
    
    free_test_client(user);
    free_test_server(server);
}

// Test with multiple words in message
Test(broadcast, broadcast_multiple_words_message)
{
    server_t *server = create_test_server(10, 10);
    client_t *user = create_test_client(1, 5, 5, UP);
    char *buffer[] = {"broadcast", "hello", "world", "this", "is", "a", "test", NULL};
    
    user->client_fd = 1;
    server->client = user;
    user->next = NULL;
    
    broadcast(server, user, buffer);
    // Should concatenate all words into a single message
    
    server->client = NULL;
    free_test_client(user);
    free_test_server(server);
}
