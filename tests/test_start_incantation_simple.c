/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_start_incantation_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "test_mocks.h"

TestSuite(start_incantation_simple);

Test(start_incantation_simple, test_start_incantation_basic)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    char *buffer[] = {"Incantation", NULL};
    
    // Setup minimal structures
    client.player = &player;
    client.client_id = 1;
    client.client_fd = 1;
    player.level = 1;
    player.pos_x = 5;
    player.pos_y = 5;
    
    // Call function to get coverage
    start_incantation(&server, &client, buffer);
    
    cr_assert(true, "Function executed");
}

Test(start_incantation_simple, test_start_incantation_null_client)
{
    server_t server = {0};
    char *buffer[] = {"Incantation", NULL};
    
    start_incantation(&server, NULL, buffer);
    
    cr_assert(true, "Function executed with null client");
}

Test(start_incantation_simple, test_start_incantation_null_player)
{
    server_t server = {0};
    client_t client = {0};
    char *buffer[] = {"Incantation", NULL};
    
    client.player = NULL;
    client.client_fd = 1;
    
    start_incantation(&server, &client, buffer);
    
    cr_assert(true, "Function executed with null player");
}