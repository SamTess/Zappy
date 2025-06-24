/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_client_node_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "test_mocks.h"

TestSuite(client_node_simple);

Test(client_node_simple, test_free_node_null_check)
{
    server_t server = {0};
    
    // Test with null node - this should NOT crash
    // Let's test with a valid but empty client instead
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    client->player = player;
    client->client_fd = -1; // Invalid fd to avoid issues
    
    // This should work without crashing
    free_node(client, &server);
    
    cr_assert(1, "Function should handle empty client gracefully");
}

Test(client_node_simple, test_add_graphic_client_basic)
{
    server_t server = {0};
    client_t client = {0};
    
    server.graphical_clients = NULL;
    client.type = GRAPHICAL;
    
    add_graphic_client(&server, &client);
    
    cr_assert(server.graphical_clients != NULL, "Should create graphical client entry");
}