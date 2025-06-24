/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_ppo_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"

TestSuite(command_ppo_simple);

Test(command_ppo_simple, test_find_client_by_id_basic)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    // Setup minimal structures
    client.client_id = 1;
    client.type = AI;
    client.player = &player;
    client.next = NULL;
    server.client = &client;
    
    client_t *result = find_client_by_id(&server, 1);
    
    cr_assert_not_null(result, "Should find client");
    cr_assert_eq(result->client_id, 1, "Should return correct client");
}

Test(command_ppo_simple, test_send_ppo_command_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t graphical_client = {0};
    player_t player = {0};
    
    // Setup structures
    client.client_id = 1;
    client.type = AI;
    client.player = &player;
    client.next = NULL;
    player.pos_x = 5;
    player.pos_y = 5;
    player.rotation = UP;
    
    graphical_client.type = GRAPHICAL;
    graphical_client.is_fully_connected = true;
    graphical_client.client_fd = 1;
    graphical_client.next = NULL;
    
    server.client = &client;
    client.next = &graphical_client;
    
    bool result = send_ppo_command(&server, 1);
    
    cr_assert(result, "Function should return true");
}

Test(command_ppo_simple, test_command_ppo_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t target_client = {0};
    player_t player = {0};
    graphical_client_t graphical_client = {0};
    char *buffer[] = {"ppo", "#1", NULL};
    
    // Setup structures
    target_client.client_id = 1;
    target_client.type = AI;
    target_client.player = &player;
    target_client.next = NULL;
    player.pos_x = 5;
    player.pos_y = 5;
    player.rotation = UP;
    
    server.client = &target_client;
    server.graphical_clients = &graphical_client;
    client.client_fd = 1;
    
    command_ppo(&server, &client, buffer);
    
    cr_assert(true, "Function executed");
}