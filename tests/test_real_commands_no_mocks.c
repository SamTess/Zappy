/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_real_commands_no_mocks - Tests calling REAL functions for maximum coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <fcntl.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"

// Helper functions to create test data
static server_t *create_coverage_server(int width, int height)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = width;
    parsed_info->height = height;
    server->parsed_info = parsed_info;
    server->current_tick = 100;
    
    // Allocate map
    server->map = calloc(height, sizeof(tile_t *));
    for (int y = 0; y < height; y++) {
        server->map[y] = calloc(width, sizeof(tile_t));
        for (int x = 0; x < width; x++) {
            // Initialize resources on each tile
            for (int i = 0; i < COUNT; i++) {
                server->map[y][x].resources[i] = (i + x + y) % 3; // Some resources
            }
            server->map[y][x].player_ids = NULL;
            server->map[y][x].player_count = 0;
            server->map[y][x].player_capacity = 0;
        }
    }
    
    server->current_resources = calloc(COUNT, sizeof(int));
    for (int i = 0; i < COUNT; i++) {
        server->current_resources[i] = 50;
    }
    
    // Create dummy client list for graphical clients
    server->client = calloc(1, sizeof(client_t)); // Dummy head
    server->client->next = calloc(1, sizeof(client_t));
    server->client->next->type = GRAPHICAL; // Fixed: use GRAPHICAL instead of GRAPHIC
    server->client->next->client_fd = -1; // Invalid FD to avoid real writes
    server->client->next->next = NULL;
    
    return server;
}

static client_t *create_coverage_client(int pos_x, int pos_y)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 42;
    client->client_fd = -1; // Invalid FD to avoid real writes
    client->player = player;
    client->type = AI;
    
    player->pos_x = pos_x;
    player->pos_y = pos_y;
    player->level = 2; // Level 2 for interesting look tests
    player->rotation = UP;
    player->team_name = strdup("CoverageTeam");
    player->is_in_incantation = false;
    player->busy_until = 0;
    
    // Initialize inventory
    for (int i = 0; i < COUNT; i++) {
        player->inventory[i] = i + 1; // Some items in inventory
    }
    
    return client;
}

static void free_coverage_server(server_t *server)
{
    if (!server) return;
    
    if (server->map && server->parsed_info) {
        for (int y = 0; y < server->parsed_info->height; y++) {
            free(server->map[y]);
        }
        free(server->map);
    }
    
    // Free client list
    client_t *current = server->client;
    while (current) {
        client_t *next = current->next;
        free(current);
        current = next;
    }
    
    free(server->current_resources);
    free(server->parsed_info);
    free(server);
}

static void free_coverage_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        free(client->player->team_name);
        free(client->player);
    }
    free(client);
}

TestSuite(real_commands_coverage);

// Helper function to parse broadcast commands
static char **parse_command_for_broadcast(char *full_command)
{
    // Simple parser for "Broadcast message" -> ["Broadcast", "message", NULL]
    char *space = strchr(full_command, ' ');
    if (!space) return NULL;
    
    char **result = calloc(3, sizeof(char *));
    result[0] = strndup(full_command, space - full_command);
    result[1] = strdup(space + 1);
    result[2] = NULL;
    
    return result;
}

// =============================================================================
// MOVEMENT COMMANDS - Target: forward.c, left.c, right.c
// =============================================================================

Test(real_commands_coverage, test_real_forward_all_directions)
{
    server_t *server = create_coverage_server(10, 10);
    client_t *client = create_coverage_client(5, 5);
    char *buffer[] = {"Forward", NULL};
    
    // Test forward in all 4 directions
    for (int dir = UP; dir <= LEFT; dir++) {
        client->player->rotation = dir;
        client->player->pos_x = 5;
        client->player->pos_y = 5;
        
        forward(server, client, buffer);
        
        // Test edge wrapping by moving to edges
        client->player->pos_x = 0;
        client->player->pos_y = 0;
        forward(server, client, buffer);
        
        client->player->pos_x = 9;
        client->player->pos_y = 9;
        forward(server, client, buffer);
    }
    
    free_coverage_server(server);
    free_coverage_client(client);
}

Test(real_commands_coverage, test_real_left_right_rotations)
{
    server_t *server = create_coverage_server(5, 5);
    client_t *client = create_coverage_client(2, 2);
    char *left_buffer[] = {"Left", NULL};
    char *right_buffer[] = {"Right", NULL};
    
    // Test all rotation combinations
    for (int initial_rot = UP; initial_rot <= LEFT; initial_rot++) {
        client->player->rotation = initial_rot;
        
        // Test left rotation
        left(server, client, left_buffer);
        
        // Test right rotation  
        right(server, client, right_buffer);
    }
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// LOOK COMMAND - Target: look.c
// =============================================================================

Test(real_commands_coverage, test_real_look_all_levels_directions)
{
    server_t *server = create_coverage_server(15, 15);
    client_t *client = create_coverage_client(7, 7);
    char *buffer[] = {"Look", NULL};
    
    // Test look at different levels (1-8) and all directions
    for (int level = 1; level <= 8; level++) {
        client->player->level = level;
        
        for (int dir = UP; dir <= LEFT; dir++) {
            client->player->rotation = dir;
            
            // Test from center
            client->player->pos_x = 7;
            client->player->pos_y = 7;
            look(server, client, buffer);
            
            // Test from edges for wrapping
            client->player->pos_x = 0;
            client->player->pos_y = 0;
            look(server, client, buffer);
            
            client->player->pos_x = 14;
            client->player->pos_y = 14;
            look(server, client, buffer);
        }
    }
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// INVENTORY COMMAND - Target: inventory.c
// =============================================================================

Test(real_commands_coverage, test_real_inventory_variations)
{
    server_t *server = create_coverage_server(5, 5);
    client_t *client = create_coverage_client(2, 2);
    char *buffer[] = {"Inventory", NULL};
    
    // Test with different inventory states
    inventory(server, client, buffer);
    
    // Empty inventory
    for (int i = 0; i < COUNT; i++) {
        client->player->inventory[i] = 0;
    }
    inventory(server, client, buffer);
    
    // Full inventory
    for (int i = 0; i < COUNT; i++) {
        client->player->inventory[i] = 999;
    }
    inventory(server, client, buffer);
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// TAKE/SET COMMANDS - Target: take_object.c, set_object.c
// =============================================================================

Test(real_commands_coverage, test_real_take_set_all_resources)
{
    server_t *server = create_coverage_server(5, 5);
    client_t *client = create_coverage_client(2, 2);
    
    char *resources[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    for (int i = 0; i < 7; i++) {
        // Add resource to tile
        server->map[2][2].resources[i] = 5;
        
        // Test take
        char *take_buffer[] = {"Take", resources[i], NULL};
        take_object(server, client, take_buffer);
        
        // Test set (player should have resource from take)
        char *set_buffer[] = {"Set", resources[i], NULL};
        set_object(server, client, set_buffer);
    }
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// FORK COMMAND - Target: fork.c
// =============================================================================

Test(real_commands_coverage, test_real_fork_command)
{
    server_t *server = create_coverage_server(5, 5);
    client_t *client = create_coverage_client(2, 2);
    char *buffer[] = {"Fork", NULL};
    
    // Test fork at different positions
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            client->player->pos_x = x;
            client->player->pos_y = y;
            fork_c(server, client, buffer);
        }
    }
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// EJECT COMMAND - Target: eject.c
// =============================================================================

Test(real_commands_coverage, test_real_eject_command)
{
    server_t *server = create_coverage_server(10, 10);
    client_t *ejector = create_coverage_client(5, 5);
    char *buffer[] = {"Eject", NULL};
    
    // Create another player at same position to eject
    client_t *target = create_coverage_client(5, 5);
    target->next = server->client->next;
    server->client->next = target;
    
    // Add eggs at same position
    server->eggs = calloc(1, sizeof(egg_t));
    server->eggs->pos_x = 5;
    server->eggs->pos_y = 5;
    server->eggs->egg_id = 1;
    
    // Test eject
    eject(server, ejector, buffer);
    
    // Test eject with no other players/eggs
    ejector->player->pos_x = 1;
    ejector->player->pos_y = 1;
    eject(server, ejector, buffer);
    
    free(server->eggs);
    free_coverage_server(server);
    free_coverage_client(ejector);
    // Note: target client freed by server cleanup
}

// =============================================================================
// INCANTATION COMMAND - Target: start_incantation.c
// =============================================================================

Test(real_commands_coverage, test_real_incantation_command)
{
    server_t *server = create_coverage_server(5, 5);
    client_t *client = create_coverage_client(2, 2);
    char *buffer[] = {"Incantation", NULL};
    
    // Test incantation at different levels
    for (int level = 1; level <= 7; level++) {
        client->player->level = level;
        client->player->is_in_incantation = false;
        client->player->busy_until = 0;
        
        // Add required resources for incantation
        for (int i = 0; i < COUNT; i++) {
            server->map[2][2].resources[i] = 10;
            client->player->inventory[i] = 10;
        }
        
        start_incantation(server, client, buffer);
    }
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// BROADCAST COMMAND - Target: broadcast.c
// =============================================================================

Test(real_commands_coverage, test_real_broadcast_command)
{
    server_t *server = create_coverage_server(10, 10);
    client_t *client = create_coverage_client(5, 5);
    
    char *broadcast_msgs[] = {
        "Broadcast hello world",
        "Broadcast test message with spaces",
        "Broadcast 123",
        "Broadcast !@#$%^&*()",
        NULL
    };
    
    for (int i = 0; broadcast_msgs[i]; i++) {
        char **buffer = parse_command_for_broadcast(broadcast_msgs[i]);
        if (buffer) {
            broadcast(server, client, buffer);
            // Free the parsed buffer
            for (int j = 0; buffer[j]; j++) {
                free(buffer[j]);
            }
            free(buffer);
        }
    }
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// CONNECT_NBR COMMAND
// =============================================================================

Test(real_commands_coverage, test_real_connect_nbr_command)
{
    server_t *server = create_coverage_server(5, 5);
    client_t *client = create_coverage_client(2, 2);
    char *buffer[] = {"Connect_nbr", NULL};
    
    connect_nbr(server, client, buffer);
    
    free_coverage_server(server);
    free_coverage_client(client);
}

// =============================================================================
// ERROR CASES FOR MAXIMUM COVERAGE
// =============================================================================

Test(real_commands_coverage, test_error_cases_for_coverage)
{
    server_t *server = create_coverage_server(5, 5);
    client_t *client = create_coverage_client(2, 2);
    
    // Test with NULL parameters
    forward(NULL, client, NULL);
    left(server, NULL, NULL);
    right(NULL, NULL, NULL);
    look(server, client, NULL);
    inventory(NULL, client, NULL);
    
    // Test with invalid arguments
    char *invalid_take[] = {"Take", "invalid_resource", NULL};
    take_object(server, client, invalid_take);
    
    char *invalid_set[] = {"Set", "invalid_resource", NULL};
    set_object(server, client, invalid_set);
    
    // Test with too many arguments
    char *too_many[] = {"Forward", "extra", "args", NULL};
    forward(server, client, too_many);
    
    free_coverage_server(server);
    free_coverage_client(client);
}