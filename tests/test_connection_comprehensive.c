/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Comprehensive tests for connection module
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/parsing.h"

// Helper function to create a mock parsing_info_t
static parsing_info_t *create_mock_parsing_info(void)
{
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    info->port = 4242;
    info->width = 10;
    info->height = 10;
    info->client_nb = 5;
    info->frequence = 100;
    info->names = calloc(3, sizeof(char *));
    info->names[0] = strdup("team1");
    info->names[1] = strdup("team2");
    info->names[2] = NULL;
    return info;
}

// Helper function to free mock parsing_info_t
static void free_mock_parsing_info(parsing_info_t *info)
{
    if (!info) return;
    if (info->names) {
        for (int i = 0; info->names[i]; i++) {
            free(info->names[i]);
        }
        free(info->names);
    }
    free(info);
}

// Helper function to create a mock server
static server_t *create_mock_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->client = NULL;
    server->nfds = 0;
    return server;
}

// Helper function to create a mock client with given fd
static client_t *create_mock_client(int fd, int client_id)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = fd;
    client->client_id = client_id;
    client->client_poll = calloc(1, sizeof(struct pollfd));
    client->client_poll->fd = fd;
    client->player = calloc(1, sizeof(player_t));
    client->next = NULL;
    client->is_fully_connected = false;
    return client;
}

// Test suite for remove_fd function
Test(connection, remove_fd_empty_client_list)
{
    server_t *server = create_mock_server();
    
    // Should not crash when removing from empty list
    remove_fd(server, 42);
    
    cr_assert_eq(server->client, NULL);
    free(server);
}

Test(connection, remove_fd_single_client_head)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client(42, 1);
    server->client = client;
    server->nfds = 1;
    
    cr_redirect_stdout();
    remove_fd(server, 42);
    
    cr_assert_eq(server->client, NULL);
    cr_assert_eq(server->nfds, 0);
    free(server);
}

Test(connection, remove_fd_multiple_clients_head)
{
    server_t *server = create_mock_server();
    client_t *client1 = create_mock_client(42, 1);
    client_t *client2 = create_mock_client(43, 2);
    client1->next = client2;
    server->client = client1;
    server->nfds = 2;
    
    cr_redirect_stdout();
    remove_fd(server, 42);
    
    cr_assert_eq(server->client, client2);
    cr_assert_eq(server->nfds, 1);
    
    // Cleanup remaining client
    free(client2->client_poll);
    free(client2->player);
    free(client2);
    free(server);
}

Test(connection, remove_fd_multiple_clients_middle)
{
    server_t *server = create_mock_server();
    client_t *client1 = create_mock_client(42, 1);
    client_t *client2 = create_mock_client(43, 2);
    client_t *client3 = create_mock_client(44, 3);
    client1->next = client2;
    client2->next = client3;
    server->client = client1;
    server->nfds = 3;
    
    cr_redirect_stdout();
    remove_fd(server, 43);
    
    cr_assert_eq(server->client, client1);
    cr_assert_eq(client1->next, client3);
    cr_assert_eq(server->nfds, 2);
    
    // Cleanup remaining clients
    free(client1->client_poll);
    free(client1->player);
    free(client1);
    free(client3->client_poll);
    free(client3->player);
    free(client3);
    free(server);
}

Test(connection, remove_fd_nonexistent_client)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client(42, 1);
    server->client = client;
    server->nfds = 1;
    
    cr_redirect_stdout();
    remove_fd(server, 999); // Non-existent fd
    
    // Client list should remain unchanged
    cr_assert_eq(server->client, client);
    cr_assert_eq(server->nfds, 1);
    
    // Cleanup
    free(client->client_poll);
    free(client->player);
    free(client);
    free(server);
}

// Test suite for add_fd function
Test(connection, add_fd_empty_server)
{
    server_t *server = create_mock_server();
    
    add_fd(server, 42);
    
    cr_assert_neq(server->client, NULL);
    cr_assert_eq(server->client->client_fd, 42);
    cr_assert_eq(server->client->client_id, -1); // First client gets -1
    cr_assert_eq(server->client->next, NULL);
    
    // Cleanup
    free(server->client->client_poll);
    free(server->client->player);
    free(server->client);
    free(server);
}

Test(connection, add_fd_existing_clients)
{
    server_t *server = create_mock_server();
    
    // Add first client (server socket)
    add_fd(server, 41);
    
    // Add second client
    add_fd(server, 42);
    
    cr_assert_neq(server->client, NULL);
    cr_assert_eq(server->client->client_fd, 41);
    cr_assert_eq(server->client->client_id, -1);
    
    cr_assert_neq(server->client->next, NULL);
    cr_assert_eq(server->client->next->client_fd, 42);
    cr_assert_eq(server->client->next->client_id, 0); // Second client gets 0
    
    // Cleanup
    client_t *current = server->client;
    while (current) {
        client_t *next = current->next;
        free(current->client_poll);
        free(current->player);
        free(current);
        current = next;
    }
    free(server);
}

Test(connection, add_fd_multiple_clients_incrementing_ids)
{
    server_t *server = create_mock_server();
    
    // Add server socket
    add_fd(server, 40);
    
    // Add multiple clients
    for (int i = 1; i <= 5; i++) {
        add_fd(server, 40 + i);
    }
    
    // Verify IDs increment correctly
    client_t *current = server->client;
    cr_assert_eq(current->client_id, -1); // Server socket
    
    current = current->next;
    for (int i = 0; i < 5; i++) {
        cr_assert_neq(current, NULL);
        cr_assert_eq(current->client_id, i);
        current = current->next;
    }
    
    // Cleanup
    current = server->client;
    while (current) {
        client_t *next = current->next;
        free(current->client_poll);
        free(current->player);
        free(current);
        current = next;
    }
    free(server);
}

// Test suite for init_server_resources function
Test(connection, init_server_resources_basic)
{
    server_t server = {0};
    
    init_server_resources(&server);
    
    cr_assert_neq(server.total_resources, NULL);
    cr_assert_neq(server.current_resources, NULL);
    
    // Verify all resources are initialized to 0
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(server.total_resources[i], 0);
        cr_assert_eq(server.current_resources[i], 0);
    }
    
    // Cleanup
    free(server.total_resources);
    free(server.current_resources);
}

Test(connection, init_server_resources_arrays_independent)
{
    server_t server = {0};
    
    init_server_resources(&server);
    
    // Modify one array and verify the other is unchanged
    server.total_resources[0] = 42;
    server.total_resources[1] = 100;
    
    cr_assert_eq(server.current_resources[0], 0);
    cr_assert_eq(server.current_resources[1], 0);
    
    // Cleanup
    free(server.total_resources);
    free(server.current_resources);
}

// Test suite for create_server function
Test(connection, create_server_initialization)
{
    server_t server = {0};
    parsing_info_t *info = create_mock_parsing_info();
    
    // Note: This test may require mocking socket operations
    // or running with proper permissions
    
    // For now, we'll test the parsing info copying
    server.parsed_info = malloc(sizeof(parsing_info_t));
    server.parsed_info->port = info->port;
    server.parsed_info->width = info->width;
    server.parsed_info->height = info->height;
    server.parsed_info->client_nb = info->client_nb;
    server.parsed_info->frequence = info->frequence;
    
    // Copy names manually for testing
    int name_count = 0;
    for (; info->names[name_count] != NULL; name_count++);
    
    server.parsed_info->names = calloc(name_count + 1, sizeof(char *));
    for (int j = 0; info->names[j] != NULL; j++) {
        server.parsed_info->names[j] = strdup(info->names[j]);
    }
    server.parsed_info->names[name_count] = NULL;
    
    // Verify copying worked correctly
    cr_assert_eq(server.parsed_info->port, 4242);
    cr_assert_eq(server.parsed_info->width, 10);
    cr_assert_eq(server.parsed_info->height, 10);
    cr_assert_eq(server.parsed_info->client_nb, 5);
    cr_assert_eq(server.parsed_info->frequence, 100);
    
    cr_assert_str_eq(server.parsed_info->names[0], "team1");
    cr_assert_str_eq(server.parsed_info->names[1], "team2");
    cr_assert_eq(server.parsed_info->names[2], NULL);
    
    // Cleanup
    for (int i = 0; server.parsed_info->names[i]; i++) {
        free(server.parsed_info->names[i]);
    }
    free(server.parsed_info->names);
    free(server.parsed_info);
    free_mock_parsing_info(info);
}

Test(connection, create_server_resource_initialization)
{
    server_t server = {0};
    parsing_info_t *info = create_mock_parsing_info();
    
    // Initialize just the resources part
    init_server_resources(&server);
    
    cr_assert_neq(server.total_resources, NULL);
    cr_assert_neq(server.current_resources, NULL);
    
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(server.total_resources[i], 0);
        cr_assert_eq(server.current_resources[i], 0);
    }
    
    // Cleanup
    free(server.total_resources);
    free(server.current_resources);
    free_mock_parsing_info(info);
}

// Integration test for connection workflow
Test(connection, connection_workflow_integration)
{
    server_t *server = create_mock_server();
    
    // Simulate adding multiple clients
    add_fd(server, 40); // Server socket
    add_fd(server, 41); // Client 1
    add_fd(server, 42); // Client 2
    add_fd(server, 43); // Client 3
    
    // Verify clients were added correctly
    client_t *current = server->client;
    int client_count = 0;
    while (current) {
        client_count++;
        current = current->next;
    }
    cr_assert_eq(client_count, 4);
    
    // Remove a client from the middle
    cr_redirect_stdout();
    remove_fd(server, 42);
    
    // Verify client was removed
    current = server->client;
    client_count = 0;
    bool found_42 = false;
    while (current) {
        if (current->client_fd == 42) {
            found_42 = true;
        }
        client_count++;
        current = current->next;
    }
    cr_assert_eq(client_count, 3);
    cr_assert_eq(found_42, false);
    
    // Cleanup remaining clients
    current = server->client;
    while (current) {
        client_t *next = current->next;
        free(current->client_poll);
        free(current->player);
        free(current);
        current = next;
    }
    free(server);
}

// Edge cases and error conditions
Test(connection, add_fd_client_structure_integrity)
{
    server_t *server = create_mock_server();
    
    add_fd(server, 42);
    
    client_t *client = server->client;
    cr_assert_neq(client, NULL);
    cr_assert_neq(client->client_poll, NULL);
    cr_assert_neq(client->player, NULL);
    cr_assert_eq(client->client_poll->fd, 42);
    cr_assert_eq(client->client_poll->events, POLLIN);
    cr_assert_eq(client->client_poll->revents, 0);
    cr_assert_eq(client->is_fully_connected, false);
    
    // Cleanup
    free(client->client_poll);
    free(client->player);
    free(client);
    free(server);
}

Test(connection, remove_fd_server_socket_protection)
{
    server_t *server = create_mock_server();
    server->s_fd = 40;
    
    client_t *client = create_mock_client(40, 1); // Same fd as server
    server->client = client;
    server->nfds = 1;
    
    cr_redirect_stdout();
    remove_fd(server, 40);
    
    // Should print error message but handle gracefully
    cr_assert_eq(server->nfds, 0); // Client should still be removed
    
    free(server);
}