/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_right_and_set_object_coverage
*/

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/tile.h"

TestSuite(right_and_set_object_coverage);

static server_t *create_test_server_complete(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    server->parsed_info = parsed_info;
    
    // Create map
    server->map = calloc(10, sizeof(tile_t *));
    for (int i = 0; i < 10; i++) {
        server->map[i] = calloc(10, sizeof(tile_t));
        for (int j = 0; j < 10; j++) {
            tile_init(&server->map[i][j]);
        }
    }
    
    // Initialize current_resources array
    server->current_resources = calloc(COUNT, sizeof(int));
    
    return server;
}

static client_t *create_test_client_complete(int id)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = id;
    client->client_fd = id + 10;
    client->player = player;
    client->type = AI;
    client->is_fully_connected = true;
    
    player->pos_x = 5;
    player->pos_y = 5;
    player->rotation = UP;
    player->level = 1;
    player->team_name = strdup("team1");
    
    // Initialize inventory
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 0;
    }
    player->inventory[FOOD] = 10; // Give some food
    player->inventory[LINEMATE] = 5; // Give some linemate
    
    return client;
}

static void free_test_server_complete(server_t *server)
{
    if (!server) return;
    
    if (server->map) {
        for (int i = 0; i < 10; i++) {
            if (server->map[i]) {
                free(server->map[i]);
            }
        }
        free(server->map);
    }
    
    if (server->parsed_info) {
        free(server->parsed_info);
    }
    
    if (server->current_resources) {
        free(server->current_resources);
    }
    
    free(server);
}

static void free_test_client_complete(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        if (client->player->team_name) {
            free(client->player->team_name);
        }
        free(client->player);
    }
    free(client);
}

// Tests for right command
Test(right_and_set_object_coverage, test_right_basic_rotation)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"right", NULL};
    
    // Test rotation from UP to RIGHT
    client->player->rotation = UP;
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, RIGHT);
    
    // Test rotation from RIGHT to DOWN
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, DOWN);
    
    // Test rotation from DOWN to LEFT
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, LEFT);
    
    // Test rotation from LEFT to UP (wrap around)
    right(server, client, buffer);
    cr_assert_eq(client->player->rotation, UP);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_right_null_parameters)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"right", NULL};
    
    // Test with NULL server
    right(NULL, client, buffer);
    
    // Test with NULL client
    right(server, NULL, buffer);
    
    // Test with NULL buffer
    right(server, client, NULL);
    
    // Test with NULL player
    client_t client_no_player = *client;
    client_no_player.player = NULL;
    right(server, &client_no_player, buffer);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_right_invalid_buffer)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    
    // Test with wrong number of arguments
    char *wrong_buffer[] = {"right", "extra", NULL};
    right(server, client, wrong_buffer);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

// Tests for set_object command
Test(right_and_set_object_coverage, test_set_object_valid_food)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"set", "food", NULL};
    
    // Ensure player has food in inventory
    client->player->inventory[FOOD] = 5;
    int initial_map_food = server->map[client->player->pos_y][client->player->pos_x].resources[FOOD];
    int initial_current_resources = server->current_resources[FOOD];
    
    set_object(server, client, buffer);
    
    // Check that food was removed from inventory
    cr_assert_eq(client->player->inventory[FOOD], 4);
    
    // Check that food was added to map
    cr_assert_eq(server->map[client->player->pos_y][client->player->pos_x].resources[FOOD], initial_map_food + 1);
    
    // Check that current resources were updated
    cr_assert_eq(server->current_resources[FOOD], initial_current_resources + 1);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_set_object_valid_linemate)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"set", "linemate", NULL};
    
    // Ensure player has linemate in inventory
    client->player->inventory[LINEMATE] = 3;
    int initial_map_linemate = server->map[client->player->pos_y][client->player->pos_x].resources[LINEMATE];
    int initial_current_resources = server->current_resources[LINEMATE];
    
    set_object(server, client, buffer);
    
    // Check that linemate was removed from inventory
    cr_assert_eq(client->player->inventory[LINEMATE], 2);
    
    // Check that linemate was added to map
    cr_assert_eq(server->map[client->player->pos_y][client->player->pos_x].resources[LINEMATE], initial_map_linemate + 1);
    
    // Check that current resources were updated
    cr_assert_eq(server->current_resources[LINEMATE], initial_current_resources + 1);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_set_object_null_parameters)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"set", "food", NULL};
    
    // Test with NULL server
    set_object(NULL, client, buffer);
    
    // Test with NULL client
    set_object(server, NULL, buffer);
    
    // Test with NULL buffer
    set_object(server, client, NULL);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_set_object_null_player)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"set", "food", NULL};
    
    // Test with NULL player
    client_t client_no_player = *client;
    client_no_player.player = NULL;
    set_object(server, &client_no_player, buffer);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_set_object_wrong_args)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    
    // Test with wrong number of arguments
    char *wrong_buffer1[] = {"set", NULL};
    set_object(server, client, wrong_buffer1);
    
    char *wrong_buffer2[] = {"set", "food", "extra", NULL};
    set_object(server, client, wrong_buffer2);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_set_object_invalid_resource)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"set", "invalid_resource", NULL};
    
    set_object(server, client, buffer);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}

Test(right_and_set_object_coverage, test_set_object_no_resource_in_inventory)
{
    server_t *server = create_test_server_complete();
    client_t *client = create_test_client_complete(1);
    char *buffer[] = {"set", "deraumere", NULL};
    
    // Ensure player has no deraumere in inventory
    client->player->inventory[DERAUMERE] = 0;
    
    set_object(server, client, buffer);
    
    // Inventory should remain unchanged
    cr_assert_eq(client->player->inventory[DERAUMERE], 0);
    
    free_test_client_complete(client);
    free_test_server_complete(server);
}