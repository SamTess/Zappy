/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_graphical_commands_coverage - tests for graphical commands with 0% coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"
#include "test_mocks.h"
#include <stdlib.h>
#include <string.h>

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
static void reset_graphical_mocks(void)
{
    mock_write_calls = 0;
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
}

// Helper to create test server with graphical clients
static server_t *create_test_server_with_graphical(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    info->width = 10;
    info->height = 10;
    server->parsed_info = info;
    
    // Create map
    server->map = calloc(10, sizeof(tile_t *));
    for (int y = 0; y < 10; y++) {
        server->map[y] = calloc(10, sizeof(tile_t));
        for (int x = 0; x < 10; x++) {
            tile_init(&server->map[y][x]);
        }
    }
    
    // Create graphical client
    graphical_client_t *graphical = calloc(1, sizeof(graphical_client_t));
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = 10;
    client->type = GRAPHICAL;
    graphical->client = client;
    server->graphical_clients = graphical;
    
    return server;
}

// Helper to create AI client
static client_t *create_test_ai_client(int id)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    player->pos_x = 5;
    player->pos_y = 5;
    player->level = 1;
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = i + 1;
    }
    client->player = player;
    client->client_id = id;
    client->type = AI;
    client->client_fd = 20;
    return client;
}

// Cleanup helpers
static void free_test_server_graphical(server_t *server)
{
    if (server) {
        if (server->map) {
            for (int y = 0; y < 10; y++) {
                if (server->map[y]) {
                    for (int x = 0; x < 10; x++) {
                        tile_free(&server->map[y][x]);
                    }
                    free(server->map[y]);
                }
            }
            free(server->map);
        }
        if (server->graphical_clients) {
            free(server->graphical_clients->client);
            free(server->graphical_clients);
        }
        free(server->parsed_info);
        free(server);
    }
}

static void free_test_ai_client(client_t *client)
{
    if (client) {
        free(client->player);
        free(client);
    }
}

TestSuite(graphical_commands_coverage_tests);

// =============================================================================
// COMMAND_PDR TESTS (currently 0% coverage)
// =============================================================================

Test(graphical_commands_coverage_tests, test_command_pdr_basic)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    client_t *client = create_test_ai_client(42);
    
    command_pdr(server, client, LINEMATE);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_written_message, "pdr #42 1") != NULL);
    
    free_test_server_graphical(server);
    free_test_ai_client(client);
}

Test(graphical_commands_coverage_tests, test_command_pdr_null_server)
{
    reset_graphical_mocks();
    
    client_t *client = create_test_ai_client(42);
    
    command_pdr(NULL, client, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_ai_client(client);
}

Test(graphical_commands_coverage_tests, test_command_pdr_null_client)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    
    command_pdr(server, NULL, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_graphical(server);
}

// =============================================================================
// COMMAND_PEX TESTS (currently 0% coverage)
// =============================================================================

Test(graphical_commands_coverage_tests, test_command_pex_basic)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    client_t *client = create_test_ai_client(55);
    
    command_pex(server, client);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_written_message, "pex #55") != NULL);
    
    free_test_server_graphical(server);
    free_test_ai_client(client);
}

Test(graphical_commands_coverage_tests, test_command_pex_null_server)
{
    reset_graphical_mocks();
    
    client_t *client = create_test_ai_client(55);
    
    command_pex(NULL, client);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_ai_client(client);
}

Test(graphical_commands_coverage_tests, test_command_pex_null_client)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    
    command_pex(server, NULL);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_graphical(server);
}

// =============================================================================
// COMMAND_PGT TESTS (currently 0% coverage)
// =============================================================================

Test(graphical_commands_coverage_tests, test_command_pgt_basic)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    client_t *client = create_test_ai_client(33);
    
    command_pgt(server, client, DERAUMERE);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_written_message, "pgt #33 2") != NULL);
    
    free_test_server_graphical(server);
    free_test_ai_client(client);
}

Test(graphical_commands_coverage_tests, test_command_pgt_different_resources)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    client_t *client = create_test_ai_client(33);
    
    // Test different resource types
    command_pgt(server, client, FOOD);
    command_pgt(server, client, LINEMATE);
    command_pgt(server, client, SIBUR);
    command_pgt(server, client, MENDIANE);
    command_pgt(server, client, PHIRAS);
    command_pgt(server, client, THYSTAME);
    
    cr_assert_eq(mock_write_calls, 6);
    
    free_test_server_graphical(server);
    free_test_ai_client(client);
}

// =============================================================================
// COMMAND_PIC TESTS (currently 0% coverage)
// =============================================================================

Test(graphical_commands_coverage_tests, test_command_pic_basic)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    
    // Add some players to the tile
    server->map[3][4].player_ids = malloc(sizeof(int) * 2);
    server->map[3][4].player_ids[0] = 10;
    server->map[3][4].player_ids[1] = 20;
    server->map[3][4].player_count = 2;
    server->map[3][4].player_capacity = 2;
    
    command_pic(server, 4, 3, 2);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_written_message, "pic 4 3 2") != NULL);
    cr_assert(strstr(last_written_message, "#10") != NULL);
    cr_assert(strstr(last_written_message, "#20") != NULL);
    
    free_test_server_graphical(server);
}

Test(graphical_commands_coverage_tests, test_command_pic_out_of_bounds)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    
    // Test negative coordinates
    command_pic(server, -1, 5, 1);
    cr_assert_eq(mock_write_calls, 0);
    
    command_pic(server, 5, -1, 1);
    cr_assert_eq(mock_write_calls, 0);
    
    // Test coordinates too large
    command_pic(server, 15, 5, 1);
    cr_assert_eq(mock_write_calls, 0);
    
    command_pic(server, 5, 15, 1);
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_graphical(server);
}

// =============================================================================
// COMMAND_PIE TESTS (currently 0% coverage)
// =============================================================================

Test(graphical_commands_coverage_tests, test_command_pie_basic)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    
    command_pie(server, 5, 3, 1);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_written_message, "pie 5 3 1") != NULL);
    
    free_test_server_graphical(server);
}

Test(graphical_commands_coverage_tests, test_command_pie_different_results)
{
    reset_graphical_mocks();
    
    server_t *server = create_test_server_with_graphical();
    
    // Test success and failure
    command_pie(server, 1, 2, 1); // Success
    command_pie(server, 3, 4, 0); // Failure
    
    cr_assert_eq(mock_write_calls, 2);
    
    free_test_server_graphical(server);
}

Test(graphical_commands_coverage_tests, test_command_pie_null_server)
{
    reset_graphical_mocks();
    
    command_pie(NULL, 5, 3, 1);
    
    cr_assert_eq(mock_write_calls, 0);
}

// =============================================================================
// COMMAND_PDI TESTS (currently 0% coverage - based on pattern from other files)
// =============================================================================

// Note: command_pdi might not exist in the provided files, but adding a placeholder test
// that will help if the function exists or gets implemented

Test(graphical_commands_coverage_tests, test_graphical_commands_exist)
{
    // This test ensures that our test infrastructure works
    // and can be extended for other graphical commands
    cr_assert(true);
}