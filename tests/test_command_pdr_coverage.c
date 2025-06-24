#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"

// Mock functions
void write_command_output(int fd, const char *message) {
    (void)fd;
    (void)message;
}

Test(command_pdr, test_command_pdr_valid_client)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    graphical_client_t graphical_client = {0};
    client_t graphical_cli = {0};
    
    // Setup
    client.client_id = 42;
    client.player = &player;
    client.client_fd = 5;
    
    graphical_cli.client_fd = 10;
    graphical_client.client = &graphical_cli;
    graphical_client.next = NULL;
    
    server.graphical_clients = &graphical_client;
    
    // Test with FOOD resource
    command_pdr(&server, &client, FOOD);
    
    // Test with LINEMATE resource
    command_pdr(&server, &client, LINEMATE);
    
    cr_assert(1); // If we reach here, no segfault occurred
}

Test(command_pdr, test_command_pdr_null_client)
{
    server_t server = {0};
    graphical_client_t graphical_client = {0};
    
    server.graphical_clients = &graphical_client;
    
    // Test with NULL client
    command_pdr(&server, NULL, FOOD);
    
    cr_assert(1); // Should handle gracefully
}

Test(command_pdr, test_command_pdr_null_player)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    
    server.graphical_clients = &graphical_client;
    client.player = NULL; // NULL player
    
    command_pdr(&server, &client, FOOD);
    
    cr_assert(1); // Should handle gracefully
}

Test(command_pdr, test_command_pdr_no_graphical_clients)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    server.graphical_clients = NULL; // No graphical clients
    
    command_pdr(&server, &client, FOOD);
    
    cr_assert(1); // Should handle gracefully
}

Test(command_pdr, test_command_pdr_multiple_graphical_clients)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    graphical_client_t graphical1 = {0};
    graphical_client_t graphical2 = {0};
    client_t graphical_cli1 = {0};
    client_t graphical_cli2 = {0};
    
    // Setup client
    client.client_id = 100;
    client.player = &player;
    client.client_fd = 5;
    
    // Setup first graphical client
    graphical_cli1.client_fd = 10;
    graphical1.client = &graphical_cli1;
    graphical1.next = &graphical2;
    
    // Setup second graphical client
    graphical_cli2.client_fd = 11;
    graphical2.client = &graphical_cli2;
    graphical2.next = NULL;
    
    server.graphical_clients = &graphical1;
    
    // Test with all resource types
    command_pdr(&server, &client, DERAUMERE);
    command_pdr(&server, &client, SIBUR);
    command_pdr(&server, &client, MENDIANE);
    command_pdr(&server, &client, PHIRAS);
    command_pdr(&server, &client, THYSTAME);
    
    cr_assert(1);
}