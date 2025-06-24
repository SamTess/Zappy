/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_right_coverage - tests for right command (0% coverage)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "test_mocks.h"
#include <stdlib.h>

// Mock variables
static int mock_write_calls = 0;
static char *last_written_message = NULL;

// Mock write function
void __wrap_write_command_output(int fd, char *message)
{
    (void)fd;
    mock_write_calls++;
    if (last_written_message) free(last_written_message);
    last_written_message = strdup(message);
}

// Reset mocks
static void reset_right_mocks(void)
{
    mock_write_calls = 0;
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
}

// Helper to create test client with player
static client_t *create_test_client_right(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    player->orientation = NORTH;
    player->pos_x = 5;
    player->pos_y = 5;
    client->player = player;
    client->client_fd = 10;
    client->client_id = 42;
    return client;
}

// Helper to create test server
static server_t *create_test_server_right(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    info->freq = 100;
    server->parsed_info = info;
    return server;
}

// Cleanup helpers
static void free_test_client_right(client_t *client)
{
    if (client) {
        free(client->player);
        free(client);
    }
}

static void free_test_server_right(server_t *server)
{
    if (server) {
        free(server->parsed_info);
        free(server);
    }
}

TestSuite(command_right_coverage_tests);

Test(command_right_coverage_tests, test_right_from_north)
{
    reset_right_mocks();
    
    server_t *server = create_test_server_right();
    client_t *client = create_test_client_right();
    client->player->orientation = NORTH;
    
    right(server, client);
    
    cr_assert_eq(client->player->orientation, EAST);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_written_message, "ok\n");
    
    free_test_server_right(server);
    free_test_client_right(client);
}

Test(command_right_coverage_tests, test_right_from_east)
{
    reset_right_mocks();
    
    server_t *server = create_test_server_right();
    client_t *client = create_test_client_right();
    client->player->orientation = EAST;
    
    right(server, client);
    
    cr_assert_eq(client->player->orientation, SOUTH);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_written_message, "ok\n");
    
    free_test_server_right(server);
    free_test_client_right(client);
}

Test(command_right_coverage_tests, test_right_from_south)
{
    reset_right_mocks();
    
    server_t *server = create_test_server_right();
    client_t *client = create_test_client_right();
    client->player->orientation = SOUTH;
    
    right(server, client);
    
    cr_assert_eq(client->player->orientation, WEST);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_written_message, "ok\n");
    
    free_test_server_right(server);
    free_test_client_right(client);
}

Test(command_right_coverage_tests, test_right_from_west)
{
    reset_right_mocks();
    
    server_t *server = create_test_server_right();
    client_t *client = create_test_client_right();
    client->player->orientation = WEST;
    
    right(server, client);
    
    cr_assert_eq(client->player->orientation, NORTH);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_written_message, "ok\n");
    
    free_test_server_right(server);
    free_test_client_right(client);
}

Test(command_right_coverage_tests, test_right_null_server)
{
    reset_right_mocks();
    
    client_t *client = create_test_client_right();
    orientation_t original_orientation = client->player->orientation;
    
    right(NULL, client);
    
    // Should not change orientation or write anything
    cr_assert_eq(client->player->orientation, original_orientation);
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_client_right(client);
}

Test(command_right_coverage_tests, test_right_null_client)
{
    reset_right_mocks();
    
    server_t *server = create_test_server_right();
    
    right(server, NULL);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_right(server);
}

Test(command_right_coverage_tests, test_right_null_player)
{
    reset_right_mocks();
    
    server_t *server = create_test_server_right();
    client_t *client = calloc(1, sizeof(client_t));
    client->player = NULL;
    client->client_fd = 10;
    
    right(server, client);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_right(server);
    free(client);
}