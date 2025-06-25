/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_tick_update_complete_coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/pending_cmd_utils.h"
#include "../src/Server/include/tile.h"
#include "test_mocks.h"

TestSuite(tick_update_complete_coverage);

Test(tick_update_complete_coverage, test_update_game_tick_basic)
{
    server_t server = {0};
    server.current_tick = 0;
    server.client = NULL;
    
    update_game_tick(&server);
    
    cr_assert_eq(server.current_tick, 1);
}

Test(tick_update_complete_coverage, test_update_game_tick_with_clients)
{
    server_t server = {0};
    client_t dummy_client = {0}; // First client (sentinel)
    client_t client1 = {0};
    client_t client2 = {0};
    player_t player1 = {0};
    player_t player2 = {0};
    
    // Setup client chain
    dummy_client.next = &client1;
    client1.next = &client2;
    client2.next = NULL;
    
    // Setup players
    client1.player = &player1;
    client2.player = &player2;
    player1.busy_until = 5;
    player2.busy_until = 10;
    player1.queue_size = 1;
    player2.queue_size = 0;
    
    server.client = &dummy_client;
    server.current_tick = 0;
    
    update_game_tick(&server);
    
    cr_assert_eq(server.current_tick, 1);
}

Test(tick_update_complete_coverage, test_update_game_tick_queued_commands)
{
    server_t server = {0};
    client_t dummy_client = {0};
    client_t client1 = {0};
    player_t player1 = {0};
    
    dummy_client.next = &client1;
    client1.next = NULL;
    client1.player = &player1;
    
    player1.pending_cmd = NULL;
    player1.busy_until = 5;
    player1.queue_size = 2;
    
    server.client = &dummy_client;
    server.current_tick = 10; // Greater than busy_until
    
    update_game_tick(&server);
    
    cr_assert_eq(server.current_tick, 11);
}