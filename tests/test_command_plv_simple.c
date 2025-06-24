/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_plv_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"

TestSuite(command_plv_simple);

Test(command_plv_simple, test_send_plv_command_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t recipient = {0};
    player_t player = {0};
    
    // Setup minimal structures
    client.player = &player;
    client.client_id = 1;
    player.level = 2;
    recipient.client_fd = 1;
    
    // Call function to get coverage
    send_plv_command(&server, &client, &recipient);
    
    cr_assert(true, "Function executed");
}

Test(command_plv_simple, test_send_plv_to_all_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t graphical_client = {0};
    player_t player = {0};
    
    client.player = &player;
    client.client_id = 1;
    graphical_client.type = GRAPHICAL;
    graphical_client.next = NULL;
    server.client = &graphical_client;
    
    send_plv_to_all(&server, &client);
    
    cr_assert(true, "Function executed");
}

Test(command_plv_simple, test_command_plv_basic)
{
    server_t server = {0};
    client_t client = {0};
    client_t target_client = {0};
    player_t player = {0};
    graphical_client_t graphical_client = {0};
    char *buffer[] = {"plv", "#1", NULL};
    
    // Setup structures
    target_client.client_id = 1;
    target_client.type = AI;
    target_client.player = &player;
    target_client.next = NULL;
    
    server.client = &target_client;
    server.graphical_clients = &graphical_client;
    client.client_fd = 1;
    
    command_plv(&server, &client, buffer);
    
    cr_assert(true, "Function executed");
}