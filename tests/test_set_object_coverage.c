/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_set_object_coverage - tests for set_object command to improve coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "test_mocks.h"

TestSuite(set_object_coverage);

// External mock variables
extern int mock_write_calls;
extern int mock_pdr_calls;
extern char *last_message;

static void reset_set_mocks(void)
{
    mock_write_calls = 0;
    mock_pdr_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Helper to create test server
static server_t *create_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    server->parsed_info = parsed_info;
    
    // Create map
    server->map = calloc(10, sizeof(tile_t *));
    for (int y = 0; y < 10; y++) {
        server->map[y] = calloc(10, sizeof(tile_t));
    }
    
    // Initialize current_resources
    server->current_resources = calloc(COUNT, sizeof(int));
    
    return server;
}

// Helper to create test client
static client_t *create_test_client(int pos_x, int pos_y)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 1;
    client->client_fd = 5;
    client->player = player;
    
    player->pos_x = pos_x;
    player->pos_y = pos_y;
    
    // Initialize inventory to 0
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 0;
    }
    
    return client;
}

static void free_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->map) {
        for (int y = 0; y < 10; y++) {
            free(server->map[y]);
        }
        free(server->map);
    }
    
    free(server->current_resources);
    free(server->parsed_info);
    free(server);
}

static void free_test_client(client_t *client)
{
    if (!client) return;
    free(client->player);
    free(client);
}

// Test: set_object with NULL server
Test(set_object_coverage, test_set_null_server)
{
    client_t *client = create_test_client(0, 0);
    char *buffer[] = {"Set", "food", NULL};
    
    reset_set_mocks();
    
    set_object(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_client(client);
}

// Test: set_object with NULL client
Test(set_object_coverage, test_set_null_client)
{
    server_t *server = create_test_server();
    char *buffer[] = {"Set", "food", NULL};
    
    reset_set_mocks();
    
    set_object(server, NULL, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
}

// Test: set_object with NULL player
Test(set_object_coverage, test_set_null_player)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(0, 0);
    char *buffer[] = {"Set", "food", NULL};
    
    // Set player to NULL
    free(client->player);
    client->player = NULL;
    
    reset_set_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free(client);
}

// Test: set_object with invalid buffer length
Test(set_object_coverage, test_set_invalid_buffer)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(0, 0);
    char *buffer[] = {"Set", NULL};
    
    reset_set_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test: set_object with invalid resource type
Test(set_object_coverage, test_set_invalid_resource)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(2, 2);
    char *buffer[] = {"Set", "invalid", NULL};
    
    reset_set_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_pdr_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: set_object with no resource in inventory
Test(set_object_coverage, test_set_no_resource)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(2, 2);
    char *buffer[] = {"Set", "food", NULL};
    
    // Player has no food (already 0)
    reset_set_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_pdr_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: set_object with food success
Test(set_object_coverage, test_set_food_success)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(2, 2);
    char *buffer[] = {"Set", "food", NULL};
    
    // Give player food
    client->player->inventory[FOOD] = 3;
    server->current_resources[FOOD] = 10;
    
    reset_set_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    cr_assert_eq(mock_pdr_calls, 1);
    
    // Check inventory decreased
    cr_assert_eq(client->player->inventory[FOOD], 2);
    
    // Check tile increased
    cr_assert_eq(server->map[2][2].resources[FOOD], 1);
    
    // Check global resources increased
    cr_assert_eq(server->current_resources[FOOD], 11);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: set_object with all resource types
Test(set_object_coverage, test_set_all_resources)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(1, 1);
    
    char *resources[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    // Give player all resource types
    for (int i = 0; i < COUNT; i++) {
        client->player->inventory[i] = 2;
        server->current_resources[i] = 5;
    }
    
    reset_set_mocks();
    
    for (int i = 0; i < COUNT; i++) {
        char *buffer[] = {"Set", resources[i], NULL};
        set_object(server, client, buffer);
        
        // Check each resource was set correctly
        cr_assert_eq(client->player->inventory[i], 1);
        cr_assert_eq(server->map[1][1].resources[i], 1);
        cr_assert_eq(server->current_resources[i], 6);
    }
    
    cr_assert_eq(mock_write_calls, COUNT);
    cr_assert_eq(mock_pdr_calls, COUNT);
    
    free_test_server(server);
    free_test_client(client);
}