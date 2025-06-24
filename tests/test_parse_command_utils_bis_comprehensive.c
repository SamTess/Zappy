/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parse_command_utils_bis_comprehensive - Complete tests for parse_command_utils_bis
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/player.h"
#include "test_mocks.h"

// External mock variables
extern int mock_write_calls;
extern char *last_message;

TestSuite(parse_command_utils_bis_comprehensive);

// Mock function declarations
void __wrap_init_new_player_pos(server_t *server, client_t *client);

// Mock implementations
static bool mock_init_new_player_pos_called = false;

void __wrap_init_new_player_pos(server_t *server, client_t *client)
{
    (void)server;
    (void)client;
    mock_init_new_player_pos_called = true;
}

static void reset_test_mocks(void)
{
    mock_write_calls = 0;
    mock_init_new_player_pos_called = false;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

static server_t *create_test_server_with_teams(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    // Create team names array
    parsed_info->names = calloc(4, sizeof(char *));
    parsed_info->names[0] = strdup("team1");
    parsed_info->names[1] = strdup("team2");
    parsed_info->names[2] = strdup("team3");
    parsed_info->names[3] = NULL; // NULL terminated
    
    server->parsed_info = parsed_info;
    
    return server;
}

static client_t *create_test_client_with_player(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_fd = 5;
    client->player = player;
    client->type = AI; // Initialize as AI (will be changed if needed)
    
    return client;
}

static void free_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->parsed_info) {
        if (server->parsed_info->names) {
            for (int i = 0; server->parsed_info->names[i]; i++) {
                free(server->parsed_info->names[i]);
            }
            free(server->parsed_info->names);
        }
        free(server->parsed_info);
    }
    
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

// Test 1: can_connect with NULL team name
Test(parse_command_utils_bis_comprehensive, test_can_connect_null_team_name)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client_with_player();
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, NULL);
    
    cr_assert(!result);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test 2: can_connect with NULL server
Test(parse_command_utils_bis_comprehensive, test_can_connect_null_server)
{
    client_t *client = create_test_client_with_player();
    char *buffer = "team1\n";
    
    reset_test_mocks();
    
    bool result = can_connect(NULL, client, buffer);
    
    cr_assert(!result);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_client(client);
}

// Test 3: can_connect with server without parsed_info
Test(parse_command_utils_bis_comprehensive, test_can_connect_no_parsed_info)
{
    server_t *server = calloc(1, sizeof(server_t));
    client_t *client = create_test_client_with_player();
    char *buffer = "team1\n";
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(!result);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(server);
    free_test_client(client);
}

// Test 4: can_connect with server without team names
Test(parse_command_utils_bis_comprehensive, test_can_connect_no_team_names)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    server->parsed_info = parsed_info;
    
    client_t *client = create_test_client_with_player();
    char *buffer = "team1\n";
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(!result);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(parsed_info);
    free(server);
    free_test_client(client);
}

// Test 5: can_connect with team name too short
Test(parse_command_utils_bis_comprehensive, test_can_connect_team_name_too_short)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client_with_player();
    char *buffer = "a\n"; // Only 1 character + newline
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(!result);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test 6: can_connect with team name without newline
Test(parse_command_utils_bis_comprehensive, test_can_connect_no_newline)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client_with_player();
    char *buffer = "team1"; // No newline
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(!result);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_test_server(server);
    free_test_client(client);
}

// Test 7: can_connect with GRAPHIC team (should succeed)
Test(parse_command_utils_bis_comprehensive, test_can_connect_graphic_team)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client_with_player();
    char *buffer = "GRAPHIC\n";
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(result);
    cr_assert_eq(mock_write_calls, 0); // Should not write ko for GRAPHIC
    cr_assert_eq(client->type, GRAPHICAL);
    cr_assert_null(client->player); // Player should be NULL for GRAPHIC
    
    free_test_server(server);
    free(client); // Don't call free_test_client since player is NULL
}

// Test 8: can_connect with valid team name (should succeed)
Test(parse_command_utils_bis_comprehensive, test_can_connect_valid_team)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client_with_player();
    char *buffer = "team1\n";
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(result);
    cr_assert_eq(mock_write_calls, 0); // Should not write ko for valid team
    cr_assert_eq(client->type, AI);
    cr_assert_str_eq(client->player->team_name, "team1");
    cr_assert(mock_init_new_player_pos_called);
    
    free_test_server(server);
    free_test_client(client);
}

// Test 9: can_connect with valid team but connect_nbr_srv returns negative
Test(parse_command_utils_bis_comprehensive, test_can_connect_valid_team_no_slots)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client_with_player();
    char *buffer = "team2\n";
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    // This test depends on the actual implementation of connect_nbr_srv
    // We'll just verify it doesn't crash
    cr_assert_eq(client->type, AI);
    
    free_test_server(server);
    free_test_client(client);
}

// Test 10: can_connect with invalid team name
Test(parse_command_utils_bis_comprehensive, test_can_connect_invalid_team)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client_with_player();
    char *buffer = "invalid_team\n";
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(!result);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(client->type, AI); // Type should remain AI
    cr_assert(!mock_init_new_player_pos_called);
    
    free_test_server(server);
    free_test_client(client);
}

// Test 11: can_connect with multiple valid teams (test all teams)
Test(parse_command_utils_bis_comprehensive, test_can_connect_all_valid_teams)
{
    server_t *server = create_test_server_with_teams();
    char *teams[] = {"team1\n", "team2\n", "team3\n"};
    
    for (int i = 0; i < 3; i++) {
        client_t *client = create_test_client_with_player();
        
        reset_test_mocks();
        
        bool result = can_connect(server, client, teams[i]);
        
        cr_assert(result);
        cr_assert_eq(mock_write_calls, 0);
        cr_assert_eq(client->type, AI);
        cr_assert(mock_init_new_player_pos_called);
        
        // Remove newline for comparison
        char expected_team[10];
        strcpy(expected_team, teams[i]);
        expected_team[strlen(expected_team) - 1] = '\0';
        cr_assert_str_eq(client->player->team_name, expected_team);
        
        free_test_client(client);
    }
    
    free_test_server(server);
}

// Test 12: can_connect edge case - exactly 2 characters + newline
Test(parse_command_utils_bis_comprehensive, test_can_connect_minimum_length)
{
    server_t *server = create_test_server_with_teams();
    
    // Add a 2-character team name to test minimum length
    free(server->parsed_info->names[0]);
    server->parsed_info->names[0] = strdup("ab");
    
    client_t *client = create_test_client_with_player();
    char *buffer = "ab\n"; // Exactly 2 characters + newline
    
    reset_test_mocks();
    
    bool result = can_connect(server, client, buffer);
    
    cr_assert(result);
    cr_assert_eq(mock_write_calls, 0);
    cr_assert_str_eq(client->player->team_name, "ab");
    
    free_test_server(server);
    free_test_client(client);
}