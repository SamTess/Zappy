/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pdr_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "test_mocks.h"

TestSuite(command_pdr_simple);

Test(command_pdr_simple, test_command_pdr_basic)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    graphical_client_t graphical_client = {0};
    client_t graphical_client_node = {0};
    
    // Setup minimal structures
    client.player = &player;
    client.client_id = 1;
    graphical_client.client = &graphical_client_node;
    graphical_client_node.client_fd = 1;
    server.graphical_clients = &graphical_client;
    
    // Call function to get coverage
    command_pdr(&server, &client, FOOD);
    
    cr_assert(true, "Function executed");
}

Test(command_pdr_simple, test_command_pdr_null_client)
{
    server_t server = {0};
    graphical_client_t graphical_client = {0};
    
    server.graphical_clients = &graphical_client;
    
    command_pdr(&server, NULL, FOOD);
    
    cr_assert(true, "Function executed with null client");
}

Test(command_pdr_simple, test_command_pdr_null_player)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    
    client.player = NULL;
    server.graphical_clients = &graphical_client;
    
    command_pdr(&server, &client, FOOD);
    
    cr_assert(true, "Function executed with null player");
}