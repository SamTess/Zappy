/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_look_extended - Extended tests for look command
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/tile.h"
#include "test_mocks.h"

// External mock variables
extern int mock_write_calls;
extern char *last_message;

static void reset_look_mocks(void)
{
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Helper functions
static server_t *create_look_server(int width, int height)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = width;
    parsed_info->height = height;
    server->parsed_info = parsed_info;
    
    // Allocate map with resources
    server->map = calloc(height, sizeof(tile_t *));
    for (int y = 0; y < height; y++) {
        server->map[y] = calloc(width, sizeof(tile_t));
        for (int x = 0; x < width; x++) {
            // Initialize with some basic resources
            server->map[y][x].resources[FOOD] = (x + y) % 3;
            server->map[y][x].resources[LINEMATE] = (x * y) % 2;
            for (int i = 2; i < COUNT; i++) {
                server->map[y][x].resources[i] = 0;
            }
            
            // Initialize player tracking
            server->map[y][x].player_ids = calloc(4, sizeof(int));
            server->map[y][x].player_capacity = 4;
            server->map[y][x].player_count = 0;
        }
    }
    
    return server;
}

static client_t *create_look_client(int pos_x, int pos_y, int level, enum rotation_e rotation)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 1;
    client->client_fd = 5;
    client->player = player;
    
    player->pos_x = pos_x;
    player->pos_y = pos_y;
    player->level = level;
    player->rotation = rotation;
    player->team_name = strdup("TestTeam");
    
    return client;
}

static void free_look_server(server_t *server)
{
    if (!server) return;
    
    if (server->map && server->parsed_info) {
        for (int y = 0; y < server->parsed_info->height; y++) {
            if (server->map[y]) {
                for (int x = 0; x < server->parsed_info->width; x++) {
                    free(server->map[y][x].player_ids);
                }
                free(server->map[y]);
            }
        }
        free(server->map);
    }
    
    free(server->parsed_info);
    free(server);
}

static void free_look_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

// =============================================================================
// LOOK COMMAND TESTS
// =============================================================================

Test(look_extended, test_look_null_server)
{
    client_t *client = create_look_client(5, 5, 1, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    look(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_look_client(client);
}

Test(look_extended, test_look_null_player)
{
    server_t *server = create_look_server(10, 10);
    client_t *client = create_look_client(5, 5, 1, UP);
    char *buffer[] = {"Look", NULL};
    
    // Set player to NULL
    free(client->player->team_name);
    free(client->player);
    client->player = NULL;
    
    reset_look_mocks();
    
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_look_server(server);
    free(client);
}

Test(look_extended, test_look_invalid_buffer_length)
{
    server_t *server = create_look_server(10, 10);
    client_t *client = create_look_client(5, 5, 1, UP);
    char *buffer_short[] = {NULL};
    char *buffer_long[] = {"Look", "extra", NULL};
    
    reset_look_mocks();
    
    // Test empty buffer
    look(server, client, buffer_short);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    // Test too many arguments
    look(server, client, buffer_long);
    cr_assert_eq(mock_write_calls, 2);
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_level_1_basic)
{
    server_t *server = create_look_server(10, 10);
    client_t *client = create_look_client(5, 5, 1, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    // Should return a properly formatted look result
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "[") != NULL, "Look result should start with [");
    cr_assert(strstr(last_message, "]") != NULL, "Look result should end with ]");
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_level_2_more_tiles)
{
    server_t *server = create_look_server(10, 10);
    client_t *client = create_look_client(5, 5, 2, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    // Level 2 should see more tiles (0, 1, 3, 5 tiles for levels 0, 1, 2)
    cr_assert(strstr(last_message, ",") != NULL, "Level 2 look should have commas separating levels");
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_high_level)
{
    server_t *server = create_look_server(15, 15);
    client_t *client = create_look_client(7, 7, 5, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    // High level should see many more tiles
    cr_assert(strlen(last_message) > 10, "High level look should return substantial data");
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_different_rotations)
{
    server_t *server = create_look_server(10, 10);
    client_t *client = create_look_client(5, 5, 1, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    // Test each rotation
    enum rotation_e rotations[] = {UP, DOWN, LEFT, RIGHT};
    
    for (int i = 0; i < 4; i++) {
        client->player->rotation = rotations[i];
        look(server, client, buffer);
        
        // Each rotation should succeed
        cr_assert_not_null(last_message);
        cr_assert(strstr(last_message, "[") != NULL);
        cr_assert(strstr(last_message, "]") != NULL);
    }
    
    cr_assert_eq(mock_write_calls, 4);
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_edge_positions)
{
    server_t *server = create_look_server(10, 10);
    client_t *client = create_look_client(0, 0, 2, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    // Test corner position
    look(server, client, buffer);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    
    // Test other edge positions
    client->player->pos_x = 9;
    client->player->pos_y = 9;
    look(server, client, buffer);
    
    client->player->pos_x = 0;
    client->player->pos_y = 9;
    look(server, client, buffer);
    
    client->player->pos_x = 9;
    client->player->pos_y = 0;
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 4);
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_with_resources)
{
    server_t *server = create_look_server(10, 10);
    client_t *client = create_look_client(5, 5, 1, UP);
    char *buffer[] = {"Look", NULL};
    
    // Add specific resources to current tile
    server->map[5][5].resources[FOOD] = 2;
    server->map[5][5].resources[LINEMATE] = 1;
    server->map[5][5].resources[DERAUMERE] = 1;
    
    reset_look_mocks();
    
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    // Should contain resource information
    cr_assert(strlen(last_message) > 5, "Look with resources should return more data");
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_small_map)
{
    server_t *server = create_look_server(3, 3);
    client_t *client = create_look_client(1, 1, 2, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    // Should handle small map wraparound correctly
    cr_assert(strstr(last_message, "[") != NULL);
    cr_assert(strstr(last_message, "]") != NULL);
    
    free_look_server(server);
    free_look_client(client);
}

Test(look_extended, test_look_maximum_level)
{
    server_t *server = create_look_server(20, 20);
    client_t *client = create_look_client(10, 10, 8, UP);
    char *buffer[] = {"Look", NULL};
    
    reset_look_mocks();
    
    look(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    // Maximum level should see the most tiles
    cr_assert(strlen(last_message) > 50, "Maximum level look should return substantial data");
    
    free_look_server(server);
    free_look_client(client);
}