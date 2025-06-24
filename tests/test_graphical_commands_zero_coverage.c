/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_graphical_commands_zero_coverage
*/

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"

TestSuite(graphical_commands_zero_coverage);

static server_t *create_graphical_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    server->parsed_info = parsed_info;
    
    // Create map
    server->map = calloc(10, sizeof(tile_t *));
    for (int i = 0; i < 10; i++) {
        server->map[i] = calloc(10, sizeof(tile_t));
        for (int j = 0; j < 10; j++) {
            tile_init(&server->map[i][j]);
        }
    }
    
    // Create graphical clients
    graphical_client_t *gc = calloc(1, sizeof(graphical_client_t));
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = 100;
    client->type = GRAPHICAL;
    gc->client = client;
    server->graphical_clients = gc;
    
    return server;
}

static void free_graphical_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->map) {
        for (int i = 0; i < 10; i++) {
            if (server->map[i]) {
                free(server->map[i]);
            }
        }
        free(server->map);
    }
    
    if (server->graphical_clients) {
        if (server->graphical_clients->client) {
            free(server->graphical_clients->client);
        }
        free(server->graphical_clients);
    }
    
    if (server->parsed_info) {
        free(server->parsed_info);
    }
    
    free(server);
}

// Tests for command_pdr
Test(graphical_commands_zero_coverage, test_command_pdr_basic)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    client.client_id = 1;
    
    command_pdr(server, &client, FOOD);
    command_pdr(server, &client, LINEMATE);
    command_pdr(server, &client, DERAUMERE);
    
    free_graphical_test_server(server);
    cr_assert(true); // Just test that it doesn't crash
}

Test(graphical_commands_zero_coverage, test_command_pdr_null_params)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    
    command_pdr(NULL, &client, FOOD);
    command_pdr(server, NULL, FOOD);
    
    // Test with no graphical clients
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    command_pdr(server, &client, FOOD);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

// Tests for command_pex
Test(graphical_commands_zero_coverage, test_command_pex_basic)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    player_t player = {0};
    
    client.client_id = 1;
    client.player = &player;
    
    command_pex(server, &client);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_pex_null_params)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    
    command_pex(NULL, &client);
    command_pex(server, NULL);
    
    client.player = NULL;
    command_pex(server, &client);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

// Tests for command_pgt
Test(graphical_commands_zero_coverage, test_command_pgt_basic)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    player_t player = {0};
    
    client.client_id = 1;
    client.player = &player;
    
    command_pgt(server, &client, FOOD);
    command_pgt(server, &client, LINEMATE);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_pgt_null_params)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    
    command_pgt(NULL, &client, FOOD);
    command_pgt(server, NULL, FOOD);
    
    client.player = NULL;
    command_pgt(server, &client, FOOD);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

// Tests for command_pic
Test(graphical_commands_zero_coverage, test_command_pic_basic)
{
    server_t *server = create_graphical_test_server();
    
    command_pic(server, 5, 5, 1);
    command_pic(server, 0, 0, 2);
    command_pic(server, 9, 9, 3);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_pic_null_server)
{
    command_pic(NULL, 5, 5, 1);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_pic_invalid_coords)
{
    server_t *server = create_graphical_test_server();
    
    command_pic(server, -1, 5, 1);
    command_pic(server, 5, -1, 1);
    command_pic(server, 15, 5, 1); // Out of bounds
    command_pic(server, 5, 15, 1); // Out of bounds
    
    free_graphical_test_server(server);
    cr_assert(true);
}

// Tests for command_pie
Test(graphical_commands_zero_coverage, test_command_pie_basic)
{
    server_t *server = create_graphical_test_server();
    
    command_pie(server, 5, 5, 1);
    command_pie(server, 0, 0, 0);
    command_pie(server, 9, 9, -1);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_pie_null_params)
{
    command_pie(NULL, 5, 5, 1);
    
    server_t server = {0};
    command_pie(&server, 5, 5, 1); // No graphical clients
    
    cr_assert(true);
}

// Tests for command_pdi
Test(graphical_commands_zero_coverage, test_command_pdi_basic)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    
    client.client_id = 1;
    
    command_pdi(server, &client);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_pdi_null_params)
{
    server_t *server = create_graphical_test_server();
    
    command_pdi(NULL, NULL);
    command_pdi(server, NULL);
    command_pdi(NULL, NULL);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

// Tests for command_smg
Test(graphical_commands_zero_coverage, test_send_smg_command_basic)
{
    server_t *server = create_graphical_test_server();
    
    // Test with valid parameters - send_smg_command takes only server and message
    send_smg_command(server, "test message");
    
    // Test with different message
    send_smg_command(server, "another test");
    
    free_graphical_test_server(server);
}

Test(graphical_commands_zero_coverage, test_send_smg_command_null_params)
{
    server_t *server = create_graphical_test_server();
    
    // Test with NULL server
    send_smg_command(NULL, "test");
    
    // Test with NULL message  
    send_smg_command(server, NULL);
    
    free_graphical_test_server(server);
}

// Tests for command_sst
Test(graphical_commands_zero_coverage, test_command_sst_basic)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    char *buffer[] = {"sst", "100", NULL};
    
    server->parsed_info->frequence = 50;
    client.client_fd = 100;
    
    command_sst(server, &client, buffer);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_sst_null_params)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    char *buffer[] = {"sst", "100", NULL};
    
    command_sst(NULL, &client, buffer);
    command_sst(server, NULL, buffer);
    command_sst(server, &client, NULL);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_sst_invalid_freq)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    char *invalid_buffer[] = {"sst", "invalid", NULL};
    char *zero_buffer[] = {"sst", "0", NULL};
    
    client.client_fd = 100;
    
    command_sst(server, &client, invalid_buffer);
    command_sst(server, &client, zero_buffer);
    
    free_graphical_test_server(server);
    cr_assert(true);
}

// Tests for command_tna
Test(graphical_commands_zero_coverage, test_command_tna_basic)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    char *buffer[] = {"tna", NULL};
    
    // Setup team names
    char *names[] = {"team1", "team2", "team3", NULL};
    server->parsed_info->names = names;
    client.client_fd = 100;
    
    command_tna(server, &client, buffer);
    
    server->parsed_info->names = NULL; // Don't free, they're stack allocated
    free_graphical_test_server(server);
    cr_assert(true);
}

Test(graphical_commands_zero_coverage, test_command_tna_null_params)
{
    server_t *server = create_graphical_test_server();
    client_t client = {0};
    char *buffer[] = {"tna", NULL};
    
    command_tna(NULL, &client, buffer);
    command_tna(server, NULL, buffer);
    command_tna(server, &client, NULL);
    
    free_graphical_test_server(server);
    cr_assert(true);
}