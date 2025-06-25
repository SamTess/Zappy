/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pbc_complete_coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"

TestSuite(command_pbc_complete_coverage);

Test(command_pbc_complete_coverage, test_command_pbc_null_server)
{
    client_t client = {0};
    char buffer[] = "test message";
    
    command_pbc(NULL, &client, buffer);
    // Should handle NULL server gracefully
}

Test(command_pbc_complete_coverage, test_command_pbc_null_client)
{
    server_t server = {0};
    char buffer[] = "test message";
    
    command_pbc(&server, NULL, buffer);
    // Should handle NULL client gracefully
}

Test(command_pbc_complete_coverage, test_command_pbc_null_buffer)
{
    server_t server = {0};
    client_t client = {0};
    
    command_pbc(&server, &client, NULL);
    // Should handle NULL buffer gracefully
}

Test(command_pbc_complete_coverage, test_command_pbc_no_graphical_clients)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    char buffer[] = "test message";
    
    client.player = &player;
    client.client_id = 42;
    server.graphical_clients = NULL;
    
    command_pbc(&server, &client, buffer);
    // Should handle no graphical clients gracefully
}

Test(command_pbc_complete_coverage, test_command_pbc_null_player)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    char buffer[] = "test message";
    
    client.player = NULL;
    server.graphical_clients = &graphical_client;
    
    command_pbc(&server, &client, buffer);
    // Should handle NULL player gracefully
}

Test(command_pbc_complete_coverage, test_command_pbc_successful)
{
    server_t server = {0};
    client_t client = {0};
    client_t graphical_client_socket = {0};
    graphical_client_t graphical_client1 = {0};
    graphical_client_t graphical_client2 = {0};
    player_t player = {0};
    char buffer[] = "hello world";
    
    // Setup client
    client.player = &player;
    client.client_id = 5;
    
    // Setup graphical clients
    graphical_client_socket.client_fd = 10;
    graphical_client1.client = &graphical_client_socket;
    graphical_client1.next = &graphical_client2;
    graphical_client2.client = &graphical_client_socket;
    graphical_client2.next = NULL;
    
    server.graphical_clients = &graphical_client1;
    
    command_pbc(&server, &client, buffer);
    // Should successfully send pbc command to all graphical clients
}

Test(command_pbc_complete_coverage, test_command_pbc_single_graphical_client)
{
    server_t server = {0};
    client_t client = {0};
    client_t graphical_client_socket = {0};
    graphical_client_t graphical_client = {0};
    player_t player = {0};
    char buffer[] = "single client test";
    
    client.player = &player;
    client.client_id = 1;
    
    graphical_client_socket.client_fd = 15;
    graphical_client.client = &graphical_client_socket;
    graphical_client.next = NULL;
    
    server.graphical_clients = &graphical_client;
    
    command_pbc(&server, &client, buffer);
    // Should send to single graphical client
}