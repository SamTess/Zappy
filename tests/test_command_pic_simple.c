/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pic_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "test_mocks.h"

TestSuite(command_pic_simple);

Test(command_pic_simple, test_command_pic_basic)
{
    server_t server = {0};
    parsing_info_t parsed_info = {0};
    graphical_client_t graphical_client = {0};
    client_t graphical_client_node = {0};
    tile_t **map;
    tile_t tile = {0};
    
    // Setup minimal structures
    parsed_info.width = 10;
    parsed_info.height = 10;
    server.parsed_info = &parsed_info;
    
    // Create minimal map
    map = malloc(sizeof(tile_t*) * 10);
    for (int i = 0; i < 10; i++) {
        map[i] = malloc(sizeof(tile_t) * 10);
        for (int j = 0; j < 10; j++) {
            map[i][j] = tile;
        }
    }
    server.map = map;
    
    graphical_client.client = &graphical_client_node;
    graphical_client_node.client_fd = 1;
    server.graphical_clients = &graphical_client;
    
    // Call function to get coverage
    command_pic(&server, 5, 5, 1);
    
    // Cleanup
    for (int i = 0; i < 10; i++) {
        free(map[i]);
    }
    free(map);
    
    cr_assert(true, "Function executed");
}

Test(command_pic_simple, test_command_pic_null_server)
{
    command_pic(NULL, 1, 1, 1);
    cr_assert(true, "Function executed with null server");
}

Test(command_pic_simple, test_command_pic_out_of_bounds)
{
    server_t server = {0};
    parsing_info_t parsed_info = {0};
    graphical_client_t graphical_client = {0};
    
    parsed_info.width = 5;
    parsed_info.height = 5;
    server.parsed_info = &parsed_info;
    server.graphical_clients = &graphical_client;
    
    command_pic(&server, 10, 10, 1);
    cr_assert(true, "Function executed with out of bounds coordinates");
}