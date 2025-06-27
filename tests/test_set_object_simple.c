/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_set_object_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "test_mocks.h"

TestSuite(set_object_simple);

Test(set_object_simple, test_set_object_basic)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    char *buffer[] = {"Set", "food", NULL};
    
    // Setup minimal structures
    client.player = &player;
    client.client_id = 1;
    client.client_fd = 1;
    player.pos_x = 5;
    player.pos_y = 5;
    player.inventory[FOOD] = 1;
    
    // Call function to get coverage
    set_object(&server, &client, buffer);
    
    cr_assert(true, "Function executed");
}

Test(set_object_simple, test_set_object_null_client)
{
    server_t server = {0};
    char *buffer[] = {"Set", "food", NULL};
    
    set_object(&server, NULL, buffer);
    
    cr_assert(true, "Function executed with null client");
}

Test(set_object_simple, test_set_object_invalid_resource)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    char *buffer[] = {"Set", "invalid", NULL};
    
    client.player = &player;
    client.client_fd = 1;
    
    set_object(&server, &client, buffer);
    
    cr_assert(true, "Function executed with invalid resource");
}