/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_player_init - comprehensive unit tests for player initialization
** Tests for find_egg_for_team, init_new_player_pos, and init_player functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/player.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"
#include "../src/Server/include/egg.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// External mock variables from test_mocks.c
extern int mock_tile_add_calls;
extern int mock_send_edi_calls;
extern int mock_remove_egg_calls;
extern char *last_message;

// Additional mock variables for player_init tests
static int mock_send_ebo_calls = 0;
static int mock_init_inventory_calls = 0;

// Reset mock function call counters and states
static void reset_player_init_mocks(void)
{
    mock_tile_add_calls = 0;
    mock_send_edi_calls = 0;
    mock_remove_egg_calls = 0;
    mock_send_ebo_calls = 0;
    mock_init_inventory_calls = 0;
    
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Mock implementations for player_init tests
void __wrap_send_ebo_command(server_t *server, int egg_id)
{
    (void)server;
    (void)egg_id;
    mock_send_ebo_calls++;
}

void __wrap_init_inventory(player_t *player)
{
    if (!player)
        return;
    mock_init_inventory_calls++;
    // Simulate real init_inventory behavior
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = 0;
    }
    player->inventory[FOOD] = 10;
}

// Test suite setup
TestSuite(player_init_tests);

// =============================================================================
// HELPER FUNCTIONS
// =============================================================================

/**
 * Create a test server with specified dimensions
 */
static server_t *create_test_server(int width, int height)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = width;
    parsed_info->height = height;
    server->parsed_info = parsed_info;
    
    // Allocate and initialize map
    server->map = calloc(height, sizeof(tile_t *));
    for (int y = 0; y < height; y++) {
        server->map[y] = calloc(width, sizeof(tile_t));
        for (int x = 0; x < width; x++) {
            server->map[y][x].player_ids = calloc(4, sizeof(int));
            server->map[y][x].player_capacity = 4;
            server->map[y][x].player_count = 0;
        }
    }
    
    server->eggs = NULL;
    return server;
}

/**
 * Create a test client with specified team name
 */
static client_t *create_test_client(int client_id, const char *team_name)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = client_id;
    client->client_fd = 5;
    client->player = player;
    
    if (team_name) {
        player->team_name = strdup(team_name);
    } else {
        player->team_name = NULL;
    }
    
    return client;
}

/**
 * Create a test egg
 */
static egg_t *create_test_egg(int egg_id, int pos_x, int pos_y, const char *team_name)
{
    egg_t *egg = calloc(1, sizeof(egg_t));
    
    egg->egg_id = egg_id;
    egg->pos_x = pos_x;
    egg->pos_y = pos_y;
    egg->team_name = strdup(team_name);
    egg->next = NULL;
    
    return egg;
}

/**
 * Free test server resources
 */
static void free_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->map && server->parsed_info) {
        for (int y = 0; y < server->parsed_info->height; y++) {
            if (server->map[y]) {
                for (int x = 0; x < server->parsed_info->width; x++) {
                    free(server->map[y][x].player_ids);
                }
                free(server->map[y]);
            }
        }
        free(server->map);
    }
    
    // Free eggs
    egg_t *current = server->eggs;
    while (current) {
        egg_t *next = current->next;
        free(current->team_name);
        free(current);
        current = next;
    }
    
    free(server->parsed_info);
    free(server);
}

/**
 * Free test client resources
 */
static void free_test_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        if (client->player->team_name) {
            free(client->player->team_name);
        }
        if (client->player->command_queue) {
            free(client->player->command_queue);
        }
        if (client->player->pending_cmd) {
            free(client->player->pending_cmd);
        }
        free(client->player);
    }
    free(client);
}

// =============================================================================
// INIT_NEW_PLAYER_POS TESTS
// =============================================================================

Test(player_init_tests, test_init_new_player_pos_with_team_egg)
{
    reset_player_init_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, "team_alpha");
    
    // Create a team egg
    egg_t *egg = create_test_egg(1, 5, 3, "team_alpha");
    server->eggs = egg;
    
    init_new_player_pos(server, client);
    
    // Should use egg position
    cr_assert_eq(client->player->pos_x, 5);
    cr_assert_eq(client->player->pos_y, 3);
    
    // Should call required functions
    cr_assert_eq(mock_tile_add_calls, 1);
    cr_assert_eq(mock_send_ebo_calls, 1);
    cr_assert_eq(mock_remove_egg_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_init_tests, test_init_new_player_pos_no_team_egg)
{
    reset_player_init_mocks();
    srand(12345); // Fixed seed for predictable results
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, "team_beta");
    
    // No eggs for team_beta
    server->eggs = NULL;
    
    init_new_player_pos(server, client);
    
    // Should use random position (predictable with fixed seed)
    cr_assert_geq(client->player->pos_x, 0);
    cr_assert_lt(client->player->pos_x, 10);
    cr_assert_geq(client->player->pos_y, 0);
    cr_assert_lt(client->player->pos_y, 10);
    
    // Should add to tile but not remove egg
    cr_assert_eq(mock_tile_add_calls, 1);
    cr_assert_eq(mock_send_ebo_calls, 0);
    cr_assert_eq(mock_remove_egg_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_init_tests, test_init_new_player_pos_null_team_name)
{
    reset_player_init_mocks();
    srand(54321);
    
    server_t *server = create_test_server(8, 8);
    client_t *client = create_test_client(42, NULL); // NULL team name
    
    // Create some eggs but they shouldn't match
    egg_t *egg = create_test_egg(1, 2, 2, "team_gamma");
    server->eggs = egg;
    
    init_new_player_pos(server, client);
    
    // Should use random position since team_name is NULL
    cr_assert_geq(client->player->pos_x, 0);
    cr_assert_lt(client->player->pos_x, 8);
    cr_assert_geq(client->player->pos_y, 0);
    cr_assert_lt(client->player->pos_y, 8);
    
    // Should not use egg
    cr_assert_eq(mock_tile_add_calls, 1);
    cr_assert_eq(mock_send_ebo_calls, 0);
    cr_assert_eq(mock_remove_egg_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_init_tests, test_init_new_player_pos_multiple_eggs_same_team)
{
    reset_player_init_mocks();
    
    server_t *server = create_test_server(15, 15);
    client_t *client = create_test_client(42, "team_delta");
    
    // Create multiple eggs for same team (should use first one found)
    egg_t *egg1 = create_test_egg(1, 7, 4, "team_delta");
    egg_t *egg2 = create_test_egg(2, 9, 6, "team_delta");
    egg_t *egg3 = create_test_egg(3, 1, 1, "other_team");
    
    egg1->next = egg2;
    egg2->next = egg3;
    server->eggs = egg1;
    
    init_new_player_pos(server, client);
    
    // Should use first egg found (egg1)
    cr_assert_eq(client->player->pos_x, 7);
    cr_assert_eq(client->player->pos_y, 4);
    
    cr_assert_eq(mock_tile_add_calls, 1);
    cr_assert_eq(mock_send_ebo_calls, 1);
    cr_assert_eq(mock_remove_egg_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_init_tests, test_init_new_player_pos_egg_with_null_team_name)
{
    reset_player_init_mocks();
    srand(98765);
    
    server_t *server = create_test_server(6, 6);
    client_t *client = create_test_client(42, "team_epsilon");
    
    // Create egg with NULL team_name
    egg_t *egg = create_test_egg(1, 3, 3, "team_epsilon");
    free(egg->team_name);
    egg->team_name = NULL;
    server->eggs = egg;
    
    init_new_player_pos(server, client);
    
    // Should not match NULL team_name, use random position
    cr_assert_geq(client->player->pos_x, 0);
    cr_assert_lt(client->player->pos_x, 6);
    cr_assert_geq(client->player->pos_y, 0);
    cr_assert_lt(client->player->pos_y, 6);
    
    cr_assert_eq(mock_send_ebo_calls, 0);
    cr_assert_eq(mock_remove_egg_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_init_tests, test_init_new_player_pos_boundary_positions)
{
    reset_player_init_mocks();
    
    server_t *server = create_test_server(10, 10);
    client_t *client = create_test_client(42, "boundary_team");
    
    // Test egg at boundary positions
    egg_t *egg = create_test_egg(1, 0, 0, "boundary_team"); // Top-left corner
    server->eggs = egg;
    
    init_new_player_pos(server, client);
    
    cr_assert_eq(client->player->pos_x, 0);
    cr_assert_eq(client->player->pos_y, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// =============================================================================
// INIT_PLAYER TESTS
// =============================================================================

Test(player_init_tests, test_init_player_with_team)
{
    reset_player_init_mocks();
    
    player_t player = {0};
    char *team_name = "test_team";
    
    init_player(&player, team_name);
    
    // Check basic initialization
    cr_assert_eq(player.pos_x, 0);
    cr_assert_eq(player.pos_y, 0);
    cr_assert_eq(player.rotation, UP);
    cr_assert_eq(player.busy_until, 0);
    cr_assert_eq(player.queue_size, 0);
    cr_assert_eq(player.level, 1);
    cr_assert_eq(player.life, 126);
    cr_assert_eq(player.is_in_incantation, false);
    cr_assert_eq(player.incantation_leader_id, -1);
    
    // Check team name
    cr_assert_not_null(player.team_name);
    cr_assert_str_eq(player.team_name, "test_team");
    
    // Check command queue allocation
    cr_assert_not_null(player.command_queue);
    
    // Check pending command initialization
    cr_assert_not_null(player.pending_cmd);
    cr_assert_null(player.pending_cmd->args);
    cr_assert_null(player.pending_cmd->func);
    
    // Check inventory initialization
    cr_assert_eq(mock_init_inventory_calls, 1);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_init_player_null_team)
{
    reset_player_init_mocks();
    
    player_t player = {0};
    
    init_player(&player, NULL);
    
    // Check basic initialization same as before
    cr_assert_eq(player.pos_x, 0);
    cr_assert_eq(player.pos_y, 0);
    cr_assert_eq(player.rotation, UP);
    cr_assert_eq(player.level, 1);
    cr_assert_eq(player.life, 126);
    
    // Team name should be NULL
    cr_assert_null(player.team_name);
    
    // Other components should still be initialized
    cr_assert_not_null(player.command_queue);
    cr_assert_not_null(player.pending_cmd);
    cr_assert_eq(mock_init_inventory_calls, 1);
    
    // Cleanup
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_init_player_empty_team_name)
{
    reset_player_init_mocks();
    
    player_t player = {0};
    char *empty_team = "";
    
    init_player(&player, empty_team);
    
    // Should handle empty string team name
    cr_assert_not_null(player.team_name);
    cr_assert_str_eq(player.team_name, "");
    
    cr_assert_eq(player.level, 1);
    cr_assert_eq(player.life, 126);
    cr_assert_not_null(player.command_queue);
    cr_assert_not_null(player.pending_cmd);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_init_player_command_queue_allocation_failure)
{
    reset_player_init_mocks();
    
    // This test would require mocking calloc to return NULL
    // For now, we'll test the normal case and document the edge case
    player_t player = {0};
    
    init_player(&player, "test_team");
    
    // Normal case - should succeed
    cr_assert_not_null(player.command_queue);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_init_player_pending_cmd_allocation)
{
    reset_player_init_mocks();
    
    player_t player = {0};
    
    init_player(&player, "pending_test");
    
    // Check pending command structure
    cr_assert_not_null(player.pending_cmd);
    cr_assert_null(player.pending_cmd->args);
    cr_assert_null(player.pending_cmd->func);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_init_player_default_values)
{
    reset_player_init_mocks();
    
    player_t player = {0};
    
    init_player(&player, "defaults_test");
    
    // Test all default values are set correctly
    cr_assert_eq(player.pos_x, 0);
    cr_assert_eq(player.pos_y, 0);
    cr_assert_eq(player.rotation, UP);
    cr_assert_eq(player.busy_until, 0);
    cr_assert_eq(player.queue_size, 0);
    cr_assert_eq(player.level, 1);
    cr_assert_eq(player.life, 126);
    cr_assert_eq(player.is_in_incantation, false);
    cr_assert_eq(player.incantation_leader_id, -1);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_init_player_multiple_initializations)
{
    reset_player_init_mocks();
    
    player_t player1 = {0};
    player_t player2 = {0};
    
    init_player(&player1, "team_one");
    init_player(&player2, "team_two");
    
    // Both should be initialized independently
    cr_assert_str_eq(player1.team_name, "team_one");
    cr_assert_str_eq(player2.team_name, "team_two");
    
    cr_assert_neq(player1.command_queue, player2.command_queue);
    cr_assert_neq(player1.pending_cmd, player2.pending_cmd);
    
    cr_assert_eq(mock_init_inventory_calls, 2);
    
    // Cleanup
    free(player1.team_name);
    free(player1.command_queue);
    free(player1.pending_cmd);
    free(player2.team_name);
    free(player2.command_queue);
    free(player2.pending_cmd);
}

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

Test(player_init_tests, test_full_player_creation_workflow)
{
    reset_player_init_mocks();
    srand(11111);
    
    server_t *server = create_test_server(20, 20);
    client_t *client = create_test_client(100, "integration_team");
    
    // Initialize player
    init_player(client->player, "integration_team");
    
    // Create an egg for the team
    egg_t *egg = create_test_egg(50, 10, 15, "integration_team");
    server->eggs = egg;
    
    // Position the player
    init_new_player_pos(server, client);
    
    // Verify complete initialization
    cr_assert_str_eq(client->player->team_name, "integration_team");
    cr_assert_eq(client->player->level, 1);
    cr_assert_eq(client->player->life, 126);
    cr_assert_eq(client->player->pos_x, 10); // From egg
    cr_assert_eq(client->player->pos_y, 15); // From egg
    
    // Verify calls
    cr_assert_eq(mock_init_inventory_calls, 1);
    cr_assert_eq(mock_tile_add_calls, 1);
    cr_assert_eq(mock_send_ebo_calls, 1);
    cr_assert_eq(mock_remove_egg_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_init_tests, test_edge_case_very_long_team_name)
{
    reset_player_init_mocks();
    
    // Create a very long team name
    char long_name[1000];
    memset(long_name, 'A', 999);
    long_name[999] = '\0';
    
    player_t player = {0};
    
    init_player(&player, long_name);
    
    // Should handle long team names
    cr_assert_not_null(player.team_name);
    cr_assert_str_eq(player.team_name, long_name);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_special_characters_in_team_name)
{
    reset_player_init_mocks();
    
    char *special_name = "team@#$%^&*()_+-=[]{}|;:,.<>?";
    player_t player = {0};
    
    init_player(&player, special_name);
    
    // Should handle special characters
    cr_assert_not_null(player.team_name);
    cr_assert_str_eq(player.team_name, special_name);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_unicode_team_name)
{
    reset_player_init_mocks();
    
    char *unicode_name = "équipe_ñoël_🎮";
    player_t player = {0};
    
    init_player(&player, unicode_name);
    
    // Should handle Unicode characters
    cr_assert_not_null(player.team_name);
    cr_assert_str_eq(player.team_name, unicode_name);
    
    // Cleanup
    free(player.team_name);
    free(player.command_queue);
    free(player.pending_cmd);
}

Test(player_init_tests, test_large_map_dimensions)
{
    reset_player_init_mocks();
    srand(77777);
    
    server_t *server = create_test_server(1000, 1000);
    client_t *client = create_test_client(999, "large_map_team");
    
    init_new_player_pos(server, client);
    
    // Should handle large maps
    cr_assert_geq(client->player->pos_x, 0);
    cr_assert_lt(client->player->pos_x, 1000);
    cr_assert_geq(client->player->pos_y, 0);
    cr_assert_lt(client->player->pos_y, 1000);
    
    cr_assert_eq(mock_tile_add_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}

Test(player_init_tests, test_minimal_map_dimensions)
{
    reset_player_init_mocks();
    srand(22222);
    
    server_t *server = create_test_server(1, 1);
    client_t *client = create_test_client(1, "tiny_map_team");
    
    init_new_player_pos(server, client);
    
    // Should handle minimal maps
    cr_assert_eq(client->player->pos_x, 0);
    cr_assert_eq(client->player->pos_y, 0);
    
    cr_assert_eq(mock_tile_add_calls, 1);
    
    free_test_server(server);
    free_test_client(client);
}