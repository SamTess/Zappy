/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_set_object_comprehensive - Comprehensive tests for set_object function
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/tile.h"
#include "test_mocks.h"

// External mock variables
extern int mock_write_calls;
extern char *last_message;

TestSuite(set_object_comprehensive);

// Mock implementations for functions not defined elsewhere
static bool mock_send_pin_called = false;
static resource_type_t mock_determine_type_result = FOOD;

// Mock function for send_pin_to_all (not defined elsewhere)
void __wrap_send_pin_to_all(server_t *server, client_t *client)
{
    (void)server;
    (void)client;
    mock_send_pin_called = true;
}

// Mock function for determine_type (not defined elsewhere)
resource_type_t __wrap_determine_type(char *resource_name)
{
    (void)resource_name;
    return mock_determine_type_result;
}

static void reset_test_mocks(void)
{
    mock_write_calls = 0;
    mock_send_pin_called = false;
    mock_determine_type_result = FOOD;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

static server_t *create_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    server->parsed_info = parsed_info;
    
    // Initialize map
    server->map = calloc(10, sizeof(tile_t *));
    for (int i = 0; i < 10; i++) {
        server->map[i] = calloc(10, sizeof(tile_t));
    }
    
    // Initialize current_resources
    server->current_resources = calloc(COUNT, sizeof(int));
    
    return server;
}

static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_fd = 5;
    client->player = player;
    
    player->pos_x = 3;
    player->pos_y = 4;
    player->level = 1;
    player->team_name = strdup("TestTeam");
    
    // Initialize inventory with some items
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 2; // 2 of each item
    }
    
    return client;
}

static void free_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->map) {
        for (int i = 0; i < 10; i++) {
            free(server->map[i]);
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
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

// Test 1: NULL client
Test(set_object_comprehensive, test_set_object_null_client)
{
    server_t *server = create_test_server();
    char *buffer[] = {"Set", "food", NULL};
    
    reset_test_mocks();
    
    set_object(server, NULL, buffer);
    
    cr_assert_eq(mock_write_calls, 0); // No write should occur with NULL client
    
    free_test_server(server);
}

// Test 2: NULL player
Test(set_object_comprehensive, test_set_object_null_player)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", "food", NULL};
    
    free(client->player->team_name);
    free(client->player);
    client->player = NULL;
    
    reset_test_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free(client);
}

// Test 3: Invalid buffer length (too few arguments)
Test(set_object_comprehensive, test_set_object_invalid_buffer_short)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", NULL};
    
    reset_test_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test 4: Invalid buffer length (too many arguments)
Test(set_object_comprehensive, test_set_object_invalid_buffer_long)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", "food", "extra", NULL};
    
    reset_test_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test 5: Invalid resource type
Test(set_object_comprehensive, test_set_object_invalid_resource)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", "invalid_resource", NULL};
    
    reset_test_mocks();
    mock_determine_type_result = COUNT; // Invalid resource type
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test 6: No item in inventory
Test(set_object_comprehensive, test_set_object_no_inventory)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", "food", NULL};
    
    // Set inventory to 0 to simulate no items
    client->player->inventory[FOOD] = 0;
    
    reset_test_mocks();
    mock_determine_type_result = FOOD;
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test 7: Successful set food
Test(set_object_comprehensive, test_set_object_success_food)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", "food", NULL};
    
    reset_test_mocks();
    mock_determine_type_result = FOOD;
    
    int initial_tile_food = server->map[4][3].resources[FOOD];
    int initial_total_food = server->current_resources[FOOD];
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    cr_assert(mock_send_pin_called);
    
    // Check that resources were updated
    cr_assert_eq(server->map[4][3].resources[FOOD], initial_tile_food + 1);
    cr_assert_eq(server->current_resources[FOOD], initial_total_food + 1);
    
    free_test_server(server);
    free_test_client(client);
}

// Test 8: Successful set linemate
Test(set_object_comprehensive, test_set_object_success_linemate)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", "linemate", NULL};
    
    reset_test_mocks();
    mock_determine_type_result = LINEMATE;
    
    int initial_tile_linemate = server->map[4][3].resources[LINEMATE];
    int initial_total_linemate = server->current_resources[LINEMATE];
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    cr_assert(mock_send_pin_called);
    
    // Check that resources were updated
    cr_assert_eq(server->map[4][3].resources[LINEMATE], initial_tile_linemate + 1);
    cr_assert_eq(server->current_resources[LINEMATE], initial_total_linemate + 1);
    
    free_test_server(server);
    free_test_client(client);
}

// Test 9: Set all different resource types
Test(set_object_comprehensive, test_set_object_all_resources)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    
    resource_type_t resources[] = {FOOD, LINEMATE, DERAUMERE, SIBUR, MENDIANE, PHIRAS, THYSTAME};
    char *resource_names[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    for (int i = 0; i < COUNT; i++) {
        char *buffer[] = {"Set", resource_names[i], NULL};
        
        reset_test_mocks();
        mock_determine_type_result = resources[i];
        
        int initial_tile = server->map[4][3].resources[resources[i]];
        int initial_total = server->current_resources[resources[i]];
        
        set_object(server, client, buffer);
        
        cr_assert_eq(mock_write_calls, 1);
        cr_assert_str_eq(last_message, "ok\n");
        cr_assert(mock_send_pin_called);
        
        // Check that resources were updated
        cr_assert_eq(server->map[4][3].resources[resources[i]], initial_tile + 1);
        cr_assert_eq(server->current_resources[resources[i]], initial_total + 1);
    }
    
    free_test_server(server);
    free_test_client(client);
}

// Test 10: Multiple sets at different positions
Test(set_object_comprehensive, test_set_object_different_positions)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client();
    char *buffer[] = {"Set", "food", NULL};
    
    // Test at position (3, 4)
    reset_test_mocks();
    mock_determine_type_result = FOOD;
    client->player->pos_x = 3;
    client->player->pos_y = 4;
    
    set_object(server, client, buffer);
    cr_assert_eq(server->map[4][3].resources[FOOD], 1);
    
    // Test at position (7, 2)
    reset_test_mocks();
    mock_determine_type_result = FOOD;
    client->player->pos_x = 7;
    client->player->pos_y = 2;
    
    set_object(server, client, buffer);
    cr_assert_eq(server->map[2][7].resources[FOOD], 1);
    
    // Both positions should have 1 food, total should be 2
    cr_assert_eq(server->current_resources[FOOD], 2);
    
    free_test_server(server);
    free_test_client(client);
}