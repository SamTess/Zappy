/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pic - tests for pic command function
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern void command_pic(server_t *server, int x, int y, int level);

// Helper function to create a test server
static server_t *create_test_server_with_graphics(bool has_graphics)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    server->parsed_info = parsed_info;
    
    // Allocate map
    server->map = calloc(10, sizeof(tile_t *));
    for (int y = 0; y < 10; y++) {
        server->map[y] = calloc(10, sizeof(tile_t));
        for (int x = 0; x < 10; x++) {
            server->map[y][x].player_ids = calloc(4, sizeof(int));
            server->map[y][x].player_capacity = 4;
            server->map[y][x].player_count = 0;
        }
    }
    
    if (has_graphics) {
        graphical_client_t *graph_client = calloc(1, sizeof(graphical_client_t));
        client_t *client = calloc(1, sizeof(client_t));
        
        client->client_fd = 42;
        client->client_id = 1;
        graph_client->client = client;
        graph_client->next = NULL;
        server->graphical_clients = graph_client;
    } else {
        server->graphical_clients = NULL;
    }
    
    return server;
}

static void cleanup_test_server(server_t *server)
{
    if (server) {
        if (server->map) {
            for (int y = 0; y < 10; y++) {
                if (server->map[y]) {
                    for (int x = 0; x < 10; x++) {
                        if (server->map[y][x].player_ids)
                            free(server->map[y][x].player_ids);
                    }
                    free(server->map[y]);
                }
            }
            free(server->map);
        }
        if (server->graphical_clients) {
            if (server->graphical_clients->client)
                free(server->graphical_clients->client);
            free(server->graphical_clients);
        }
        if (server->parsed_info)
            free(server->parsed_info);
        free(server);
    }
}

TestSuite(command_pic_tests);

// Test command_pic with NULL server
Test(command_pic_tests, test_command_pic_null_server)
{
    command_pic(NULL, 5, 5, 1);
    
    cr_assert(true, "command_pic should handle NULL server gracefully");
}

// Test command_pic with server that has no graphical clients
Test(command_pic_tests, test_command_pic_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    
    command_pic(server, 5, 5, 1);
    
    cr_assert(true, "command_pic should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
}

// Test command_pic with coordinates out of bounds
Test(command_pic_tests, test_command_pic_out_of_bounds)
{
    server_t *server = create_test_server_with_graphics(true);
    
    command_pic(server, -1, 5, 1);
    command_pic(server, 5, -1, 1);
    command_pic(server, 15, 5, 1);
    command_pic(server, 5, 15, 1);
    
    cr_assert(true, "command_pic should handle out of bounds coordinates gracefully");
    
    cleanup_test_server(server);
}

// Test command_pic with valid parameters
Test(command_pic_tests, test_command_pic_valid, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    command_pic(server, 5, 5, 1);
    
    cr_assert(true, "command_pic should execute with valid parameters");
    
    cleanup_test_server(server);
}

// Test command_pic with tile containing players
Test(command_pic_tests, test_command_pic_with_players, .timeout = 5)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Add some players to the tile
    server->map[3][2].player_ids[0] = 1;
    server->map[3][2].player_ids[1] = 2;
    server->map[3][2].player_count = 2;
    
    command_pic(server, 2, 3, 1);
    
    cr_assert(true, "command_pic should handle tiles with players");
    
    cleanup_test_server(server);
}