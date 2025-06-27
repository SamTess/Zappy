/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests pour tick_update.c qui est à 0% de coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"

// Mock functions
bool __wrap_check_player_starvation(server_t *server, client_t *client) {
    (void)server;
    (void)client;
    return false;
}

void __wrap_finish_incantation(server_t *server, client_t *client) {
    (void)server;
    (void)client;
}

void __wrap_respawn_resources(tile_t **map, server_t *server, int total_resources, int current_resources) {
    (void)map;
    (void)server;
    (void)total_resources;
    (void)current_resources;
}

void __wrap_execute_pending_cmd(server_t *server, client_t *client) {
    (void)server;
    (void)client;
}

void __wrap_process_next_queued_command(server_t *server, client_t *client) {
    (void)server;
    (void)client;
}

// Helper function to create a basic server
static server_t *create_test_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server) return NULL;
    
    memset(server, 0, sizeof(server_t));
    
    server->parsed_info = malloc(sizeof(parsed_args_t));
    if (!server->parsed_info) {
        free(server);
        return NULL;
    }
    memset(server->parsed_info, 0, sizeof(parsed_args_t));
    
    server->parsed_info->width = 10;
    server->parsed_info->height = 10;
    server->parsed_info->freq = 100;
    server->current_tick = 0;
    server->total_resources = 100;
    server->current_resources = 50;
    
    return server;
}

// Helper function to create a basic player
static player_t *create_test_player(void)
{
    player_t *player = malloc(sizeof(player_t));
    if (!player) return NULL;
    
    memset(player, 0, sizeof(player_t));
    player->x = 5;
    player->y = 5;
    player->level = 1;
    player->busy_until = 0;
    player->queue_size = 0;
    player->is_in_incantation = false;
    player->pending_cmd = NULL;
    
    return player;
}

// Helper function to create a basic client
static client_t *create_test_client(void)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client) return NULL;
    
    memset(client, 0, sizeof(client_t));
    client->client_fd = 1;
    client->type = AI;
    client->is_fully_connected = true;
    client->player = create_test_player();
    client->next = NULL;
    
    return client;
}

static void cleanup_server(server_t *server)
{
    if (!server) return;
    
    if (server->parsed_info) {
        free(server->parsed_info);
    }
    free(server);
}

static void cleanup_client(client_t *client)
{
    if (!client) return;
    
    if (client->player) {
        free(client->player);
    }
    free(client);
}

// Tests for tick_update.c
TestSuite(zero_coverage_tick_update);

Test(zero_coverage_tick_update, test_update_game_tick_basic)
{
    server_t *server = create_test_server();
    cr_assert_not_null(server);
    
    // Test with no clients
    server->client = NULL;
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    cleanup_server(server);
}

Test(zero_coverage_tick_update, test_update_game_tick_with_client)
{
    server_t *server = create_test_server();
    client_t *dummy_head = create_test_client();
    client_t *real_client = create_test_client();
    
    cr_assert_not_null(server);
    cr_assert_not_null(dummy_head);
    cr_assert_not_null(real_client);
    
    // Setup client chain - dummy head then real client
    dummy_head->next = real_client;
    server->client = dummy_head;
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    cleanup_client(real_client);
    cleanup_client(dummy_head);
    cleanup_server(server);
}

Test(zero_coverage_tick_update, test_update_game_tick_resource_respawn)
{
    server_t *server = create_test_server();
    cr_assert_not_null(server);
    
    // Set tick to trigger resource respawn (multiple of 20)
    server->current_tick = 19; // Will become 20 after increment
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 20);
    
    cleanup_server(server);
}

Test(zero_coverage_tick_update, test_update_game_tick_pending_cmd)
{
    server_t *server = create_test_server();
    client_t *dummy_head = create_test_client();
    client_t *real_client = create_test_client();
    
    cr_assert_not_null(server);
    cr_assert_not_null(dummy_head);
    cr_assert_not_null(real_client);
    
    // Setup client with pending command
    dummy_head->next = real_client;
    server->client = dummy_head;
    
    // Set up conditions for pending command execution
    real_client->player->pending_cmd = (void*)1; // Non-NULL pointer
    real_client->player->busy_until = 0; // Not busy
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    cleanup_client(real_client);
    cleanup_client(dummy_head);
    cleanup_server(server);
}

Test(zero_coverage_tick_update, test_update_game_tick_queued_command)
{
    server_t *server = create_test_server();
    client_t *dummy_head = create_test_client();
    client_t *real_client = create_test_client();
    
    cr_assert_not_null(server);
    cr_assert_not_null(dummy_head);
    cr_assert_not_null(real_client);
    
    // Setup client with queued commands
    dummy_head->next = real_client;
    server->client = dummy_head;
    
    // Set up conditions for queued command processing
    real_client->player->pending_cmd = NULL; // No pending command
    real_client->player->busy_until = 0; // Not busy
    real_client->player->queue_size = 2; // Has queued commands
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    cleanup_client(real_client);
    cleanup_client(dummy_head);
    cleanup_server(server);
}

Test(zero_coverage_tick_update, test_update_game_tick_incantation)
{
    server_t *server = create_test_server();
    client_t *dummy_head = create_test_client();
    client_t *real_client = create_test_client();
    
    cr_assert_not_null(server);
    cr_assert_not_null(dummy_head);
    cr_assert_not_null(real_client);
    
    // Setup client in incantation
    dummy_head->next = real_client;
    server->client = dummy_head;
    
    real_client->player->is_in_incantation = true;
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    cleanup_client(real_client);
    cleanup_client(dummy_head);
    cleanup_server(server);
}

Test(zero_coverage_tick_update, test_update_game_tick_multiple_clients)
{
    server_t *server = create_test_server();
    client_t *dummy_head = create_test_client();
    client_t *client1 = create_test_client();
    client_t *client2 = create_test_client();
    
    cr_assert_not_null(server);
    cr_assert_not_null(dummy_head);
    cr_assert_not_null(client1);
    cr_assert_not_null(client2);
    
    // Setup client chain
    dummy_head->next = client1;
    client1->next = client2;
    server->client = dummy_head;
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    cleanup_client(client2);
    cleanup_client(client1);
    cleanup_client(dummy_head);
    cleanup_server(server);
}

Test(zero_coverage_tick_update, test_update_game_tick_null_player)
{
    server_t *server = create_test_server();
    client_t *dummy_head = create_test_client();
    client_t *real_client = malloc(sizeof(client_t));
    
    cr_assert_not_null(server);
    cr_assert_not_null(dummy_head);
    cr_assert_not_null(real_client);
    
    // Setup client with NULL player
    memset(real_client, 0, sizeof(client_t));
    real_client->player = NULL;
    real_client->next = NULL;
    
    dummy_head->next = real_client;
    server->client = dummy_head;
    
    update_game_tick(server);
    
    cr_assert_eq(server->current_tick, 1);
    
    free(real_client);
    cleanup_client(dummy_head);
    cleanup_server(server);
}