/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_connect_nbr_additional - Additional tests to improve connect_nbr coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/egg.h"
#include "test_mocks.h"

TestSuite(connect_nbr_additional);

// External mock variables
extern int mock_write_calls;
extern char *last_message;

static server_t *create_server_with_config(int client_nb)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->client_nb = client_nb;
    server->parsed_info = parsed_info;
    server->client = calloc(1, sizeof(client_t)); // Dummy head
    server->eggs = NULL;
    
    return server;
}

static client_t *create_client_with_team(const char *team_name)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_fd = 5;
    client->player = player;
    player->team_name = strdup(team_name);
    
    return client;
}

static egg_t *create_egg_with_team(const char *team_name)
{
    egg_t *egg = calloc(1, sizeof(egg_t));
    egg->team_name = strdup(team_name);
    egg->next = NULL;
    return egg;
}

static void free_server(server_t *server)
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
    
    // Free eggs
    egg_t *current_egg = server->eggs;
    while (current_egg) {
        egg_t *next = current_egg->next;
        free(current_egg->team_name);
        free(current_egg);
        current_egg = next;
    }
    
    free(server->parsed_info);
    free(server);
}

// Test connect_nbr_srv with various scenarios to improve coverage
Test(connect_nbr_additional, test_connect_nbr_srv_with_eggs)
{
    server_t *server = create_server_with_config(5);
    
    // Add some eggs for the team
    egg_t *egg1 = create_egg_with_team("team1");
    egg_t *egg2 = create_egg_with_team("team1");
    egg_t *egg3 = create_egg_with_team("other_team");
    
    egg1->next = egg2;
    egg2->next = egg3;
    server->eggs = egg1;
    
    // Test with team that has eggs
    int result = connect_nbr_srv(server, "team1");
    cr_assert_eq(result, 3); // 5 max - 2 eggs = 3 available
    
    // Test with team that has no eggs
    result = connect_nbr_srv(server, "team2");
    cr_assert_eq(result, 5); // 5 max - 0 eggs = 5 available
    
    free_server(server);
}

Test(connect_nbr_additional, test_connect_nbr_srv_with_players_and_eggs)
{
    server_t *server = create_server_with_config(3);
    
    // Add a player to the team
    client_t *player_client = create_client_with_team("team1");
    player_client->next = server->client->next;
    server->client->next = player_client;
    
    // Add an egg to the same team
    egg_t *egg = create_egg_with_team("team1");
    server->eggs = egg;
    
    // Test: 3 max - 1 player - 1 egg = 1 available
    int result = connect_nbr_srv(server, "team1");
    cr_assert_eq(result, 1);
    
    free_server(server);
}

Test(connect_nbr_additional, test_connect_nbr_srv_overcrowded)
{
    server_t *server = create_server_with_config(2);
    
    // Add more players and eggs than the limit
    client_t *player1 = create_client_with_team("team1");
    client_t *player2 = create_client_with_team("team1");
    client_t *player3 = create_client_with_team("team1");
    
    player1->next = player2;
    player2->next = player3;
    player3->next = server->client->next;
    server->client->next = player1;
    
    egg_t *egg = create_egg_with_team("team1");
    server->eggs = egg;
    
    // Test: 2 max - 3 players - 1 egg = -2, should return 0
    int result = connect_nbr_srv(server, "team1");
    cr_assert_eq(result, 0);
    
    free_server(server);
}

Test(connect_nbr_additional, test_connect_nbr_invalid_buffer)
{
    server_t *server = create_server_with_config(5);
    client_t *client = create_client_with_team("team1");
    
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
    
    // Test with invalid buffer (too many arguments)
    char *buffer_long[] = {"Connect_nbr", "extra", NULL};
    connect_nbr(server, client, buffer_long);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_server(server);
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

Test(connect_nbr_additional, test_connect_nbr_null_client)
{
    server_t *server = create_server_with_config(5);
    char *buffer[] = {"Connect_nbr", NULL};
    
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
    
    connect_nbr(server, NULL, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_server(server);
}

Test(connect_nbr_additional, test_connect_nbr_null_player)
{
    server_t *server = create_server_with_config(5);
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = 5;
    client->player = NULL; // NULL player
    char *buffer[] = {"Connect_nbr", NULL};
    
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
    
    connect_nbr(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_server(server);
    free(client);
}

Test(connect_nbr_additional, test_connect_nbr_null_team_name)
{
    server_t *server = create_server_with_config(5);
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_fd = 5;
    client->player = player;
    player->team_name = NULL; // NULL team name
    
    char *buffer[] = {"Connect_nbr", NULL};
    
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
    
    connect_nbr(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_server(server);
    free(player);
    free(client);
}