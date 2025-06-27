/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pnw - simple coverage tests
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
static int local_write_calls = 0;
static char *local_last_message = NULL;

// Override the mock function for this test suite
void setup_pnw_mock(void)
{
    local_write_calls = 0;
    if (local_last_message) {
        free(local_last_message);
        local_last_message = NULL;
    }
}

// Helper function to create a simple mock server
server_t *create_simple_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server) return NULL;
    
    server->client = NULL;
    server->graphical_clients = NULL;
    return server;
}

// Helper function to create a simple mock client
client_t *create_simple_client(int id, const char *team_name)
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
    
    client->player->pos_x = 5;
    client->player->pos_y = 3;
    client->player->rotation = UP;
    client->player->level = 2;
    client->player->team_name = strdup(team_name);
    
    return client;
}

// Helper function to create graphical client
client_t *create_graphical_client(void)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client) return NULL;
    
    client->client_id = 99;
    client->client_fd = 20;
    client->type = GRAPHICAL;
    client->is_fully_connected = true;
    client->next = NULL;
    client->player = NULL;
    
    return client;
}

void setup_pnw(void)
{
    setup_pnw_mock();
}

void teardown_pnw(void)
{
    if (local_last_message) {
        free(local_last_message);
        local_last_message = NULL;
    }
}

TestSuite(command_pnw, .init = setup_pnw, .fini = teardown_pnw);

Test(command_pnw, test_send_pnw_command_basic)
{
    server_t *server = create_simple_server();
    client_t *client = create_simple_client(42, "Team1");
    client_t *recipient = create_graphical_client();
    
    send_pnw_command(server, client, recipient);
    
    // The function should call write_command_output which is mocked globally
    cr_assert_geq(__wrap_write_command_output_calls, 1);
    
    free(client->player->team_name);
    free(client->player);
    free(client);
    free(recipient);
    free(server);
}

Test(command_pnw, test_send_pnw_command_null_params)
{
    int initial_calls = __wrap_write_command_output_calls;
    send_pnw_command(NULL, NULL, NULL);
    cr_assert_eq(__wrap_write_command_output_calls, initial_calls);
}

Test(command_pnw, test_send_pnw_command_to_all_basic)
{
    server_t *server = create_simple_server();
    client_t *player = create_simple_client(1, "TeamA");
    client_t *graphical = create_graphical_client();
    
    server->client = graphical;
    
    send_pnw_command_to_all(server, player);
    
    // Should call the function at least once
    cr_assert(__wrap_send_pnw_command_to_all_called);
    
    free(player->player->team_name);
    free(player->player);
    free(player);
    free(graphical);
    free(server);
}

Test(command_pnw, test_send_pnw_command_to_all_null_server)
{
    send_pnw_command_to_all(NULL, NULL);
    // Should handle null parameters gracefully
    cr_assert(true); // Basic test to ensure no crash
}

Test(command_pnw, test_send_all_player_info_null_params)
{
    send_all_player_info_to_one_client(NULL, NULL);
    // Should handle null parameters gracefully
    cr_assert(true); // Basic test to ensure no crash
}

Test(command_pnw, test_send_pnw_command_different_values)
{
    server_t *server = create_simple_server();
    client_t *client = create_simple_client(123, "MyTeam");
    client_t *recipient = create_graphical_client();
    
    client->player->pos_x = 10;
    client->player->pos_y = 7;
    client->player->rotation = DOWN;
    client->player->level = 5;
    
    int initial_calls = __wrap_write_command_output_calls;
    send_pnw_command(server, client, recipient);
    
    cr_assert_gt(__wrap_write_command_output_calls, initial_calls);
    
    free(client->player->team_name);
    free(client->player);
    free(client);
    free(recipient);
    free(server);
}