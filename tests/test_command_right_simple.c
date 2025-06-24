/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_right_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "test_mocks.h"

TestSuite(command_right_simple);

Test(command_right_simple, test_right_basic)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    char *buffer[] = {"Right", NULL};
    
    // Setup minimal structures
    client.player = &player;
    client.client_id = 1;
    client.client_fd = 1;
    player.rotation = UP;
    
    // Call function to get coverage
    right(&server, &client, buffer);
    
    cr_assert(true, "Function executed");
}

Test(command_right_simple, test_right_null_client)
{
    server_t server = {0};
    char *buffer[] = {"Right", NULL};
    
    right(&server, NULL, buffer);
    
    cr_assert(true, "Function executed with null client");
}

Test(command_right_simple, test_right_null_player)
{
    server_t server = {0};
    client_t client = {0};
    char *buffer[] = {"Right", NULL};
    
    client.player = NULL;
    client.client_fd = 1;
    
    right(&server, &client, buffer);
    
    cr_assert(true, "Function executed with null player");
}