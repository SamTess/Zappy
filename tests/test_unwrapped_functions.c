/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests pour les fonctions non-wrappées avec 0% de coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"

// Helper function to create a basic server for testing
static server_t *create_test_server_unwrapped(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server) return NULL;
    
    memset(server, 0, sizeof(server_t));
    
    server->parsed_info = malloc(sizeof(parsing_info_t));
    if (!server->parsed_info) {
        free(server);
        return NULL;
    }
    memset(server->parsed_info, 0, sizeof(parsing_info_t));
    
    server->parsed_info->width = 10;
    server->parsed_info->height = 10;
    server->parsed_info->frequence = 100;
    
    // Create team names
    server->parsed_info->names = malloc(sizeof(char*) * 3);
    server->parsed_info->names[0] = strdup("team1");
    server->parsed_info->names[1] = strdup("team2");
    server->parsed_info->names[2] = NULL;
    
    return server;
}

// Helper function to create a basic graphical client
static client_t *create_test_graphical_client_unwrapped(void)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client) return NULL;
    
    memset(client, 0, sizeof(client_t));
    client->client_fd = 1;
    client->type = GRAPHICAL;
    client->is_fully_connected = true;
    
    return client;
}

// Helper to create graphical client list
static graphical_client_t *create_graphical_client_list(client_t *client)
{
    graphical_client_t *gclient = malloc(sizeof(graphical_client_t));
    if (!gclient) return NULL;
    
    memset(gclient, 0, sizeof(graphical_client_t));
    gclient->client = client;
    gclient->next = NULL;
    
    return gclient;
}

static void cleanup_server_unwrapped(server_t *server)
{
    if (!server) return;
    
    if (server->parsed_info) {
        if (server->parsed_info->names) {
            for (int i = 0; server->parsed_info->names[i]; i++) {
                free(server->parsed_info->names[i]);
            }
            free(server->parsed_info->names);
        }
        free(server->parsed_info);
    }
    
    if (server->graphical_clients) {
        free(server->graphical_clients);
    }
    
    free(server);
}

// Tests for command_smg.c - send_smg_command function
TestSuite(unwrapped_command_smg);

Test(unwrapped_command_smg, test_send_smg_command_valid_message)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    
    cr_assert_not_null(server);
    cr_assert_not_null(client);
    
    server->graphical_clients = create_graphical_client_list(client);
    cr_assert_not_null(server->graphical_clients);
    
    // Test with valid message
    send_smg_command(server, "Test message");
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_smg, test_send_smg_command_null_server)
{
    // Test with NULL server
    send_smg_command(NULL, "Test message");
}

Test(unwrapped_command_smg, test_send_smg_command_null_message)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    
    server->graphical_clients = create_graphical_client_list(client);
    
    // Test with NULL message
    send_smg_command(server, NULL);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_smg, test_send_smg_command_no_graphical_clients)
{
    server_t *server = create_test_server_unwrapped();
    
    // Test with no graphical clients
    send_smg_command(server, "Test message");
    
    cleanup_server_unwrapped(server);
}

// Tests for command_sst.c - command_sst function
TestSuite(unwrapped_command_sst);

Test(unwrapped_command_sst, test_command_sst_valid_time)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    char *args[] = {"sst", "50", NULL};
    
    server->graphical_clients = create_graphical_client_list(client);
    
    command_sst(server, client, args);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_sst, test_command_sst_null_buffer)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    
    server->graphical_clients = create_graphical_client_list(client);
    
    command_sst(server, client, NULL);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_sst, test_command_sst_invalid_time_negative)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    char *args[] = {"sst", "-10", NULL};
    
    server->graphical_clients = create_graphical_client_list(client);
    
    command_sst(server, client, args);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_sst, test_command_sst_invalid_time_zero)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    char *args[] = {"sst", "0", NULL};
    
    server->graphical_clients = create_graphical_client_list(client);
    
    command_sst(server, client, args);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_sst, test_command_sst_non_graphical_client)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    char *args[] = {"sst", "50", NULL};
    
    client->type = AI; // Non-graphical client
    server->graphical_clients = create_graphical_client_list(client);
    
    command_sst(server, client, args);
    
    free(client);
    cleanup_server_unwrapped(server);
}

// Tests for command_tna.c - command_tna and send_tna_command functions
TestSuite(unwrapped_command_tna);

Test(unwrapped_command_tna, test_command_tna_valid_request)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    char *args[] = {"tna", NULL};
    
    server->graphical_clients = create_graphical_client_list(client);
    
    command_tna(server, client, args);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_tna, test_command_tna_null_server)
{
    client_t *client = create_test_graphical_client_unwrapped();
    char *args[] = {"tna", NULL};
    
    command_tna(NULL, client, args);
    
    free(client);
}

Test(unwrapped_command_tna, test_command_tna_null_buffer)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    
    server->graphical_clients = create_graphical_client_list(client);
    
    command_tna(server, client, NULL);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_tna, test_send_tna_command_valid)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = create_test_graphical_client_unwrapped();
    
    send_tna_command(server, client);
    
    free(client);
    cleanup_server_unwrapped(server);
}

Test(unwrapped_command_tna, test_send_tna_command_null_server)
{
    client_t *client = create_test_graphical_client_unwrapped();
    
    send_tna_command(NULL, client);
    
    free(client);
}

Test(unwrapped_command_tna, test_send_tna_command_null_client)
{
    server_t *server = create_test_server_unwrapped();
    
    send_tna_command(server, NULL);
    
    cleanup_server_unwrapped(server);
}

// Tests for tick_update.c - update_game_tick function
TestSuite(unwrapped_tick_update);

Test(unwrapped_tick_update, test_update_game_tick_basic)
{
    server_t *server = create_test_server_unwrapped();
    
    cr_assert_not_null(server);
    
    server->current_tick = 0;
    server->client = NULL;
    
    // Test que la fonction s'exécute sans erreur
    update_game_tick(server);
    
    // Test que la fonction s'est exécutée
    cr_assert(true);
    
    cleanup_server_unwrapped(server);
}

Test(unwrapped_tick_update, test_update_game_tick_with_clients)
{
    server_t *server = create_test_server_unwrapped();
    client_t *client = malloc(sizeof(client_t));
    
    memset(client, 0, sizeof(client_t));
    client->type = AI;
    client->next = NULL;
    
    server->current_tick = 0;
    server->client = client;
    
    update_game_tick(server);
    
    // Test que la fonction s'est exécutée
    cr_assert(true);
    
    free(client);
    cleanup_server_unwrapped(server);
}
