/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_start_incantation_comprehensive - Complete tests for start_incantation
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
extern int mock_command_pic_calls;
extern int last_pic_x, last_pic_y, last_pic_level;

TestSuite(start_incantation_comprehensive);

static void reset_test_mocks(void)
{
    mock_write_calls = 0;
    mock_command_pic_calls = 0;
    last_pic_x = -1;
    last_pic_y = -1;
    last_pic_level = -1;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

static server_t *create_test_server_with_tick(int current_tick)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    server->parsed_info = parsed_info;
    server->current_tick = current_tick;
    
    // Initialize map
    server->map = calloc(10, sizeof(tile_t *));
    for (int i = 0; i < 10; i++) {
        server->map[i] = calloc(10, sizeof(tile_t));
    }
    
    // Initialize client list with dummy head
    server->client = calloc(1, sizeof(client_t));
    
    return server;
}

static client_t *create_test_client_with_level(int level, int pos_x, int pos_y)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_fd = 5;
    client->client_id = 100 + level; // Unique ID
    client->player = player;
    
    player->pos_x = pos_x;
    player->pos_y = pos_y;
    player->level = level;
    player->team_name = strdup("TestTeam");
    player->is_in_incantation = false;
    player->busy_until = 0;
    player->incantation_leader_id = -1;
    
    return client;
}

static void add_client_to_server(server_t *server, client_t *client)
{
    client->next = server->client->next;
    server->client->next = client;
}

static void setup_tile_resources(server_t *server, int x, int y, int level)
{
    tile_t *tile = &server->map[y][x];
    
    // Resources required for each level (1-indexed)
    int required_resources[7][COUNT] = {
        {1, 0, 0, 0, 0, 0, 0}, // Level 1
        {1, 1, 0, 0, 0, 0, 0}, // Level 2
        {2, 1, 1, 0, 0, 0, 0}, // Level 3
        {2, 2, 1, 1, 0, 0, 0}, // Level 4
        {4, 1, 1, 2, 1, 0, 0}, // Level 5
        {4, 3, 2, 1, 1, 1, 0}, // Level 6
        {6, 1, 2, 3, 2, 2, 1}  // Level 7
    };
    
    if (level >= 1 && level <= 7) {
        for (int i = 0; i < COUNT; i++) {
            tile->resources[i] = required_resources[level - 1][i];
        }
    }
}

static void add_player_to_tile(server_t *server, int x, int y, int player_id)
{
    tile_t *tile = &server->map[y][x];
    
    if (tile->player_count < 10) { // Assuming max 10 players per tile
        tile->player_ids[tile->player_count] = player_id;
        tile->player_count++;
    }
}

static void free_test_server(server_t *server)
{
    if (!server) return;
    
    // Free client list
    client_t *current = server->client;
    while (current) {
        client_t *next = current->next;
        if (current->player) {
            free(current->player->team_name);
            free(current->player);
        }
        free(current);
        current = next;
    }
    
    // Free map
    if (server->map) {
        for (int i = 0; i < 10; i++) {
            free(server->map[i]);
        }
        free(server->map);
    }
    
    free(server->parsed_info);
    free(server);
}

// Test 1: NULL server
Test(start_incantation_comprehensive, test_start_incantation_null_server)
{
    client_t *client = create_test_client_with_level(1, 5, 5);
    char *buffer[] = {"Incantation", NULL};
    
    reset_test_mocks();
    
    start_incantation(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

// Test 2: NULL client
Test(start_incantation_comprehensive, test_start_incantation_null_client)
{
    server_t *server = create_test_server_with_tick(0);
    char *buffer[] = {"Incantation", NULL};
    
    reset_test_mocks();
    
    start_incantation(server, NULL, buffer);
    
    cr_assert_eq(mock_write_calls, 0); // Should not write with NULL client
    
    free_test_server(server);
}

// Test 3: NULL player
Test(start_incantation_comprehensive, test_start_incantation_null_player)
{
    server_t *server = create_test_server_with_tick(0);
    client_t *client = create_test_client_with_level(1, 5, 5);
    char *buffer[] = {"Incantation", NULL};
    
    free(client->player->team_name);
    free(client->player);
    client->player = NULL;
    
    reset_test_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
    free_test_server(server);
}

// Test 4: Invalid buffer length
Test(start_incantation_comprehensive, test_start_incantation_invalid_buffer)
{
    server_t *server = create_test_server_with_tick(0);
    client_t *client = create_test_client_with_level(1, 5, 5);
    char *buffer_short[] = {NULL};
    char *buffer_long[] = {"Incantation", "extra", NULL};
    
    reset_test_mocks();
    
    // Test too short
    start_incantation(server, client, buffer_short);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    // Test too long
    reset_test_mocks();
    start_incantation(server, client, buffer_long);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
}

// Test 5: Level too high (level 8)
Test(start_incantation_comprehensive, test_start_incantation_level_too_high)
{
    server_t *server = create_test_server_with_tick(0);
    client_t *client = create_test_client_with_level(8, 5, 5);
    char *buffer[] = {"Incantation", NULL};
    
    reset_test_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
}

// Test 6: Not enough resources
Test(start_incantation_comprehensive, test_start_incantation_not_enough_resources)
{
    server_t *server = create_test_server_with_tick(0);
    client_t *client = create_test_client_with_level(1, 5, 5);
    char *buffer[] = {"Incantation", NULL};
    
    add_client_to_server(server, client);
    add_player_to_tile(server, 5, 5, client->client_id);
    
    // Don't setup resources - tile will be empty
    
    reset_test_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
}

// Test 7: Not enough players
Test(start_incantation_comprehensive, test_start_incantation_not_enough_players)
{
    server_t *server = create_test_server_with_tick(0);
    client_t *client = create_test_client_with_level(2, 5, 5); // Level 2 needs 2 players
    char *buffer[] = {"Incantation", NULL};
    
    add_client_to_server(server, client);
    add_player_to_tile(server, 5, 5, client->client_id);
    setup_tile_resources(server, 5, 5, 3); // Resources for level 3 (level 2 + 1)
    
    reset_test_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
}

// Test 8: Successful incantation level 1 -> 2
Test(start_incantation_comprehensive, test_start_incantation_success_level_1)
{
    server_t *server = create_test_server_with_tick(100);
    client_t *client = create_test_client_with_level(1, 5, 5);
    char *buffer[] = {"Incantation", NULL};
    
    add_client_to_server(server, client);
    add_player_to_tile(server, 5, 5, client->client_id);
    setup_tile_resources(server, 5, 5, 2); // Resources for level 2
    
    reset_test_mocks();
    
    start_incantation(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "Elevation underway\n");
    cr_assert_eq(mock_command_pic_calls, 1);
    cr_assert_eq(last_pic_x, 5);
    cr_assert_eq(last_pic_y, 5);
    cr_assert_eq(last_pic_level, 1);
    
    // Check that player is busy
    cr_assert_eq(client->player->busy_until, 400); // 100 + 300
    cr_assert(client->player->is_in_incantation);
    cr_assert_eq(client->player->incantation_leader_id, client->client_id);
    
    free_test_server(server);
}

// Test 9: Successful incantation level 2 -> 3 (needs 2 players)
Test(start_incantation_comprehensive, test_start_incantation_success_level_2)
{
    server_t *server = create_test_server_with_tick(50);
    client_t *client1 = create_test_client_with_level(2, 3, 4);
    client_t *client2 = create_test_client_with_level(2, 3, 4);
    char *buffer[] = {"Incantation", NULL};
    
    client1->client_id = 200;
    client2->client_id = 201;
    
    add_client_to_server(server, client1);
    add_client_to_server(server, client2);
    add_player_to_tile(server, 3, 4, client1->client_id);
    add_player_to_tile(server, 3, 4, client2->client_id);
    setup_tile_resources(server, 3, 4, 3); // Resources for level 3
    
    reset_test_mocks();
    
    start_incantation(server, client1, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "Elevation underway\n");
    cr_assert_eq(mock_command_pic_calls, 1);
    
    // Both players should be busy
    cr_assert_eq(client1->player->busy_until, 350); // 50 + 300
    cr_assert(client1->player->is_in_incantation);
    cr_assert_eq(client1->player->incantation_leader_id, client1->client_id);
    
    cr_assert_eq(client2->player->busy_until, 350);
    cr_assert(client2->player->is_in_incantation);
    cr_assert_eq(client2->player->incantation_leader_id, client1->client_id);
    
    free_test_server(server);
}

// Test 10: Level 7 -> 8 (maximum level)
Test(start_incantation_comprehensive, test_start_incantation_success_level_7)
{
    server_t *server = create_test_server_with_tick(200);
    client_t *clients[6]; // Level 7 needs 6 players
    char *buffer[] = {"Incantation", NULL};
    
    // Create 6 level 7 players
    for (int i = 0; i < 6; i++) {
        clients[i] = create_test_client_with_level(7, 2, 3);
        clients[i]->client_id = 300 + i;
        add_client_to_server(server, clients[i]);
        add_player_to_tile(server, 2, 3, clients[i]->client_id);
    }
    
    setup_tile_resources(server, 2, 3, 8); // Resources for level 8 (max)
    
    reset_test_mocks();
    
    start_incantation(server, clients[0], buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "Elevation underway\n");
    cr_assert_eq(mock_command_pic_calls, 1);
    
    // All 6 players should be busy
    for (int i = 0; i < 6; i++) {
        cr_assert_eq(clients[i]->player->busy_until, 500); // 200 + 300
        cr_assert(clients[i]->player->is_in_incantation);
        cr_assert_eq(clients[i]->player->incantation_leader_id, clients[0]->client_id);
    }
    
    free_test_server(server);
}

// Test 11: Mixed level players (only same level should participate)
Test(start_incantation_comprehensive, test_start_incantation_mixed_levels)
{
    server_t *server = create_test_server_with_tick(0);
    client_t *client1 = create_test_client_with_level(3, 1, 1); // Leader
    client_t *client2 = create_test_client_with_level(3, 1, 1); // Same level
    client_t *client3 = create_test_client_with_level(2, 1, 1); // Different level
    char *buffer[] = {"Incantation", NULL};
    
    client1->client_id = 400;
    client2->client_id = 401;
    client3->client_id = 402;
    
    add_client_to_server(server, client1);
    add_client_to_server(server, client2);
    add_client_to_server(server, client3);
    add_player_to_tile(server, 1, 1, client1->client_id);
    add_player_to_tile(server, 1, 1, client2->client_id);
    add_player_to_tile(server, 1, 1, client3->client_id);
    setup_tile_resources(server, 1, 1, 4); // Resources for level 4
    
    reset_test_mocks();
    
    start_incantation(server, client1, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "Elevation underway\n");
    
    // Only level 3 players should be in incantation
    cr_assert(client1->player->is_in_incantation);
    cr_assert(client2->player->is_in_incantation);
    cr_assert(!client3->player->is_in_incantation); // Different level
    
    free_test_server(server);
}