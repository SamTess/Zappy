/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_start_incantation_coverage - tests for start_incantation command to improve coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "test_mocks.h"

TestSuite(start_incantation_coverage);

// External mock variables
extern int mock_write_calls;
extern int mock_can_start_incantation_calls;
extern int mock_start_incantation_calls;
extern int mock_pic_calls;
extern char *last_message;

static void reset_incantation_mocks(void)
{
    mock_write_calls = 0;
    mock_can_start_incantation_calls = 0;
    mock_start_incantation_calls = 0;
    mock_pic_calls = 0;
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
    
    return server;
}

// Helper to create test client
static client_t *create_test_client(int level)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 1;
    client->client_fd = 5;
    client->player = player;
    
    player->pos_x = 5;
    player->pos_y = 5;
    player->level = level;
    
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
    
    free(server->parsed_info);
    free(server);
}

static void free_test_client(client_t *client)
{
    if (!client) return;
    free(client->player);
    free(client);
}

// Test: start_incantation with NULL client
Test(start_incantation_coverage, test_start_incantation_null_client)
{
    server_t *server = create_test_server();
    char *buffer[] = {"Incantation", NULL};
    
    reset_incantation_mocks();
    
    start_incantation(server, NULL, buffer);
    
    cr_assert_eq(mock_write_calls, 0);
    cr_assert_eq(mock_can_start_incantation_calls, 0);
    
    free_test_server(server);
}

// Test: start_incantation with NULL player
Test(start_incantation_coverage, test_start_incantation_null_player)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    char *buffer[] = {"Incantation", NULL};
    
    // Set player to NULL
    free(client->player);
    client->player = NULL;
    
    reset_incantation_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_can_start_incantation_calls, 0);
    
    free_test_server(server);
    free(client);
}

// Test: start_incantation with invalid buffer length
Test(start_incantation_coverage, test_start_incantation_invalid_buffer)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    char *buffer[] = {"Incantation", "extra", NULL};
    
    reset_incantation_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_can_start_incantation_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: start_incantation with max level player
Test(start_incantation_coverage, test_start_incantation_max_level)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(8); // Max level
    char *buffer[] = {"Incantation", NULL};
    
    reset_incantation_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_can_start_incantation_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: start_incantation when can_start_incantation returns false
Test(start_incantation_coverage, test_start_incantation_cannot_start)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(2);
    char *buffer[] = {"Incantation", NULL};
    
    reset_incantation_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_can_start_incantation_calls, 1);
    cr_assert_eq(mock_start_incantation_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: start_incantation success case (will call mocked functions)
Test(start_incantation_coverage, test_start_incantation_success)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(3);
    char *buffer[] = {"Incantation", NULL};
    
    reset_incantation_mocks();
    
    // The mock for can_start_incantation will return false by default
    // So this will still result in "ko" but will exercise the path
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_eq(mock_can_start_incantation_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: start_incantation with level 1 player
Test(start_incantation_coverage, test_start_incantation_level_1)
{
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    char *buffer[] = {"Incantation", NULL};
    
    reset_incantation_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_eq(mock_can_start_incantation_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

// Test: start_incantation with various levels
Test(start_incantation_coverage, test_start_incantation_all_levels)
{
    server_t *server = create_test_server();
    char *buffer[] = {"Incantation", NULL};
    
    for (int level = 1; level <= 7; level++) {
        client_t *client = create_test_client(level);
        
        reset_incantation_mocks();
        
        start_incantation(server, client, buffer);
        
        // All should call can_start_incantation
        cr_assert_eq(mock_can_start_incantation_calls, 1);
        cr_assert_eq(mock_write_calls, 1);
        
        free_test_client(client);
    }
    
    free_test_server(server);
}