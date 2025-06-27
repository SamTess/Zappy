/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_take_set_object_extended - Extended tests for take and set object commands
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
extern int mock_pgt_calls;
extern int mock_pdr_calls;
extern char *last_message;

static void reset_extended_mocks(void)
{
    mock_write_calls = 0;
    mock_pgt_calls = 0;
    mock_pdr_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Helper functions
static server_t *create_resource_server(int width, int height)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = width;
    parsed_info->height = height;
    server->parsed_info = parsed_info;
    
    // Allocate map
    server->map = calloc(height, sizeof(tile_t *));
    for (int y = 0; y < height; y++) {
        server->map[y] = calloc(width, sizeof(tile_t));
        for (int x = 0; x < width; x++) {
            // Initialize resources to 0
            for (int i = 0; i < COUNT; i++) {
                server->map[y][x].resources[i] = 0;
            }
        }
    }
    
    // Initialize current_resources
    server->current_resources = calloc(COUNT, sizeof(int));
    
    return server;
}

static client_t *create_resource_client(int pos_x, int pos_y)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 1;
    client->client_fd = 5;
    client->player = player;
    
    player->pos_x = pos_x;
    player->pos_y = pos_y;
    player->team_name = strdup("TestTeam");
    
    // Initialize inventory
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 0;
    }
    
    return client;
}

static void free_resource_server(server_t *server)
{
    if (!server) return;
    
    if (server->map && server->parsed_info) {
        for (int y = 0; y < server->parsed_info->height; y++) {
            free(server->map[y]);
        }
        free(server->map);
    }
    
    free(server->current_resources);
    free(server->parsed_info);
    free(server);
}

static void free_resource_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

// =============================================================================
// TAKE OBJECT TESTS
// =============================================================================

Test(take_object_extended, test_take_null_server)
{
    client_t *client = create_resource_client(0, 0);
    char *buffer[] = {"Take", "food", NULL};
    
    reset_extended_mocks();
    
    take_object(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_resource_client(client);
}

Test(take_object_extended, test_take_null_player)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(0, 0);
    char *buffer[] = {"Take", "food", NULL};
    
    // Set player to NULL
    free(client->player->team_name);
    free(client->player);
    client->player = NULL;
    
    reset_extended_mocks();
    
    take_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_resource_server(server);
    free(client);
}

Test(take_object_extended, test_take_invalid_buffer_length)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(0, 0);
    char *buffer_short[] = {"Take", NULL};
    char *buffer_long[] = {"Take", "food", "extra", NULL};
    
    reset_extended_mocks();
    
    // Test too few arguments
    take_object(server, client, buffer_short);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    // Test too many arguments
    take_object(server, client, buffer_long);
    cr_assert_eq(mock_write_calls, 2);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(take_object_extended, test_take_invalid_resource_type)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(0, 0);
    char *buffer[] = {"Take", "invalid_resource", NULL};
    
    reset_extended_mocks();
    
    take_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_pgt_calls, 0);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(take_object_extended, test_take_resource_not_available)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(2, 2);
    char *buffer[] = {"Take", "food", NULL};
    
    // No resources on tile (already 0)
    reset_extended_mocks();
    
    take_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_pgt_calls, 0);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(take_object_extended, test_take_food_success)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(2, 2);
    char *buffer[] = {"Take", "food", NULL};
    
    // Add food to tile
    server->map[2][2].resources[FOOD] = 3;
    server->current_resources[FOOD] = 10;
    
    reset_extended_mocks();
    
    take_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    cr_assert_eq(mock_pgt_calls, 1);
    
    // Check resources decreased
    cr_assert_eq(server->map[2][2].resources[FOOD], 2);
    cr_assert_eq(server->current_resources[FOOD], 9);
    
    // Check inventory increased
    cr_assert_eq(client->player->inventory[FOOD], 1);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(take_object_extended, test_take_all_resource_types)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(1, 1);
    
    // Add all resource types to tile
    server->map[1][1].resources[FOOD] = 1;
    server->map[1][1].resources[LINEMATE] = 1;
    server->map[1][1].resources[DERAUMERE] = 1;
    server->map[1][1].resources[SIBUR] = 1;
    server->map[1][1].resources[MENDIANE] = 1;
    server->map[1][1].resources[PHIRAS] = 1;
    server->map[1][1].resources[THYSTAME] = 1;
    
    for (int i = 0; i < COUNT; i++) {
        server->current_resources[i] = 10;
    }
    
    char *resources[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    reset_extended_mocks();
    
    for (int i = 0; i < COUNT; i++) {
        char *buffer[] = {"Take", resources[i], NULL};
        take_object(server, client, buffer);
        
        // Each should succeed
        cr_assert_eq(client->player->inventory[i], 1);
        cr_assert_eq(server->map[1][1].resources[i], 0);
        cr_assert_eq(server->current_resources[i], 9);
    }
    
    cr_assert_eq(mock_write_calls, COUNT);
    cr_assert_eq(mock_pgt_calls, COUNT);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(take_object_extended, test_determine_type_function)
{
    // Test the determine_type function directly
    cr_assert_eq(determine_type("food"), FOOD);
    cr_assert_eq(determine_type("linemate"), LINEMATE);
    cr_assert_eq(determine_type("deraumere"), DERAUMERE);
    cr_assert_eq(determine_type("sibur"), SIBUR);
    cr_assert_eq(determine_type("mendiane"), MENDIANE);
    cr_assert_eq(determine_type("phiras"), PHIRAS);
    cr_assert_eq(determine_type("thystame"), THYSTAME);
    cr_assert_eq(determine_type("invalid"), COUNT);
    cr_assert_eq(determine_type(""), COUNT);
}

// =============================================================================
// SET OBJECT TESTS  
// =============================================================================

Test(set_object_extended, test_set_null_server)
{
    client_t *client = create_resource_client(0, 0);
    char *buffer[] = {"Set", "food", NULL};
    
    reset_extended_mocks();
    
    set_object(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_resource_client(client);
}

Test(set_object_extended, test_set_null_player)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(0, 0);
    char *buffer[] = {"Set", "food", NULL};
    
    // Set player to NULL
    free(client->player->team_name);
    free(client->player);
    client->player = NULL;
    
    reset_extended_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_resource_server(server);
    free(client);
}

Test(set_object_extended, test_set_invalid_buffer_length)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(0, 0);
    char *buffer_short[] = {"Set", NULL};
    char *buffer_long[] = {"Set", "food", "extra", NULL};
    
    reset_extended_mocks();
    
    // Test too few arguments
    set_object(server, client, buffer_short);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    // Test too many arguments
    set_object(server, client, buffer_long);
    cr_assert_eq(mock_write_calls, 2);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(set_object_extended, test_set_invalid_resource_type)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(0, 0);
    char *buffer[] = {"Set", "invalid_resource", NULL};
    
    reset_extended_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_pdr_calls, 0);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(set_object_extended, test_set_resource_not_in_inventory)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(2, 2);
    char *buffer[] = {"Set", "food", NULL};
    
    // Player has no food in inventory (already 0)
    reset_extended_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_pdr_calls, 0);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(set_object_extended, test_set_food_success)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(2, 2);
    char *buffer[] = {"Set", "food", NULL};
    
    // Add food to inventory
    client->player->inventory[FOOD] = 3;
    server->current_resources[FOOD] = 10;
    
    reset_extended_mocks();
    
    set_object(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    cr_assert_eq(mock_pdr_calls, 1);
    
    // Check resources increased
    cr_assert_eq(server->map[2][2].resources[FOOD], 1);
    cr_assert_eq(server->current_resources[FOOD], 11);
    
    // Check inventory decreased
    cr_assert_eq(client->player->inventory[FOOD], 2);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(set_object_extended, test_set_all_resource_types)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(1, 1);
    
    // Add all resource types to inventory
    for (int i = 0; i < COUNT; i++) {
        client->player->inventory[i] = 2;
        server->current_resources[i] = 10;
    }
    
    char *resources[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    reset_extended_mocks();
    
    for (int i = 0; i < COUNT; i++) {
        char *buffer[] = {"Set", resources[i], NULL};
        set_object(server, client, buffer);
        
        // Each should succeed
        cr_assert_eq(client->player->inventory[i], 1);
        cr_assert_eq(server->map[1][1].resources[i], 1);
        cr_assert_eq(server->current_resources[i], 11);
    }
    
    cr_assert_eq(mock_write_calls, COUNT);
    cr_assert_eq(mock_pdr_calls, COUNT);
    
    free_resource_server(server);
    free_resource_client(client);
}

Test(set_object_extended, test_case_sensitivity)
{
    server_t *server = create_resource_server(5, 5);
    client_t *client = create_resource_client(0, 0);
    
    // Test case sensitivity - these should fail
    char *buffer_caps[] = {"Set", "FOOD", NULL};
    char *buffer_mixed[] = {"Set", "Food", NULL};
    
    client->player->inventory[FOOD] = 1;
    
    reset_extended_mocks();
    
    set_object(server, client, buffer_caps);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    set_object(server, client, buffer_mixed);
    cr_assert_eq(mock_write_calls, 2);
    
    free_resource_server(server);
    free_resource_client(client);
}