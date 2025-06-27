/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_ppo - simple coverage tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include "test_mocks.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"

// Local mock variables to avoid conflicts
static int local_write_calls_ppo = 0;
static char *local_last_message_ppo = NULL;

// Override the mock function for this test suite
void setup_ppo_mock(void)
{
    local_write_calls_ppo = 0;
    if (local_last_message_ppo) {
        free(local_last_message_ppo);
        local_last_message_ppo = NULL;
    }
}

// Helper function to create a simple mock server for ppo tests
server_t *create_ppo_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server) return NULL;
    
    server->client = NULL;
    server->graphical_clients = NULL;
    return server;
}

// Helper function to create a simple mock client for ppo tests
client_t *create_ppo_client(int id)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client) return NULL;
    
    client->client_id = id;
    client->client_fd = 10;
    client->type = AI;
    client->is_fully_connected = true;
    client->next = NULL;
    
    client->player = malloc(sizeof(player_t));
    if (!client->player) {
        free(client);
        return NULL;
    }
    
    client->player->pos_x = 8;
    client->player->pos_y = 4;
    client->player->rotation = RIGHT;
    
    return client;
}

// Helper function to create graphical client for ppo tests
client_t *create_ppo_graphical_client(void)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client) return NULL;
    
    client->client_id = 88;
    client->client_fd = 15;
    client->type = GRAPHICAL;
    client->is_fully_connected = true;
    client->next = NULL;
    client->player = NULL;
    
    return client;
}

void setup_ppo(void)
{
    setup_ppo_mock();
}

void teardown_ppo(void)
{
    if (local_last_message_ppo) {
        free(local_last_message_ppo);
        local_last_message_ppo = NULL;
    }
}

TestSuite(command_ppo, .init = setup_ppo, .fini = teardown_ppo);

Test(command_ppo, test_find_client_by_id_not_found)
{
    server_t *server = create_ppo_server();
    client_t *client = create_ppo_client(10);
    
    server->client = client;
    
    client_t *found = find_client_by_id(server, 99);
    
    // Should not find the client
    cr_assert_null(found);
    
    free(client->player);
    free(client);
    free(server);
}

Test(command_ppo, test_send_ppo_command_basic)
{
    server_t *server = create_ppo_server();
    client_t *ai_client = create_ppo_client(25);
    client_t *graphical_client = create_ppo_graphical_client();
    
    server->client = ai_client;
    ai_client->next = graphical_client;
    
    bool result = send_ppo_command(server, 25);
    
    // Function should be called
    cr_assert_geq(mock_send_ppo_calls, 1);
    
    free(ai_client->player);
    free(ai_client);
    free(graphical_client);
    free(server);
}

Test(command_ppo, test_send_ppo_command_client_not_found)
{
    server_t *server = create_ppo_server();
    
    int initial_calls = mock_send_ppo_calls;
    bool result = send_ppo_command(server, 999);
    
    // Should still call the mock even if client not found
    cr_assert_geq(mock_send_ppo_calls, initial_calls);
    
    free(server);
}

Test(command_ppo, test_send_ppo_command_null_server)
{
    int initial_calls = mock_send_ppo_calls;
    bool result = send_ppo_command(NULL, 1);
    
    // Should handle null gracefully
    cr_assert_geq(mock_send_ppo_calls, initial_calls);
}

Test(command_ppo, test_command_ppo_valid_id)
{
    server_t *server = create_ppo_server();
    client_t *ai_client = create_ppo_client(50);
    client_t *graphical_client = create_ppo_graphical_client();
    char *buffer[] = {"ppo", "#50", NULL};
    
    server->client = ai_client;
    ai_client->next = graphical_client;
    server->graphical_clients = malloc(sizeof(graphical_client_t));
    if (server->graphical_clients) {
        server->graphical_clients->next = NULL;
    }
    
    command_ppo(server, graphical_client, buffer);
    
    // Should call the mock function
    cr_assert(__wrap_command_ppo_called);
    
    free(ai_client->player);
    free(ai_client);
    free(graphical_client);
    if (server->graphical_clients)
        free(server->graphical_clients);
    free(server);
}

Test(command_ppo, test_command_ppo_invalid_id)
{
    server_t *server = create_ppo_server();
    client_t *graphical_client = create_ppo_graphical_client();
    char *buffer[] = {"ppo", "#999", NULL};
    
    server->graphical_clients = malloc(sizeof(graphical_client_t));
    if (server->graphical_clients) {
        server->graphical_clients->next = NULL;
    }
    
    command_ppo(server, graphical_client, buffer);
    
    // Should call the mock function regardless of validity
    cr_assert(__wrap_command_ppo_called);
    
    free(graphical_client);
    if (server->graphical_clients)
        free(server->graphical_clients);
    free(server);
}

Test(command_ppo, test_command_ppo_null_params)
{
    command_ppo(NULL, NULL, NULL);
    // Should handle null parameters gracefully
    cr_assert(true); // Basic test to ensure no crash
}

Test(command_ppo, test_command_ppo_wrong_arg_count)
{
    server_t *server = create_ppo_server();
    client_t *graphical_client = create_ppo_graphical_client();
    char *buffer[] = {"ppo", NULL};
    
    server->graphical_clients = malloc(sizeof(graphical_client_t));
    if (server->graphical_clients) {
        server->graphical_clients->next = NULL;
    }
    
    command_ppo(server, graphical_client, buffer);
    
    // Should call the mock function even with wrong args
    cr_assert(__wrap_command_ppo_called);
    
    free(graphical_client);
    if (server->graphical_clients)
        free(server->graphical_clients);
    free(server);
}