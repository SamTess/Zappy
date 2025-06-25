/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pdr_pex_simple - Tests simples pour command_pdr et command_pex
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/tile.h"
#include <stdlib.h>
#include <string.h>

TestSuite(command_pdr_pex_simple);

// Variables externes définies dans test_mocks.c
extern int mock_write_calls;
extern char *last_message;

// Helper pour reset les mocks
static void reset_mocks(void)
{
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Helper pour créer un serveur de test simple
static server_t *create_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    
    info->width = 10;
    info->height = 10;
    server->parsed_info = info;
    
    // Créer un client graphique
    graphical_client_t *graphical = calloc(1, sizeof(graphical_client_t));
    client_t *graphical_client = calloc(1, sizeof(client_t));
    graphical_client->client_fd = 42;
    graphical->client = graphical_client;
    graphical->next = NULL;
    server->graphical_clients = graphical;
    
    return server;
}

// Helper pour créer un client AI de test
static client_t *create_test_client(int client_id)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = client_id;
    client->client_fd = 10;
    client->player = player;
    client->type = AI;
    
    return client;
}

// Helper pour libérer le serveur de test
static void free_test_server(server_t *server)
{
    if (server) {
        if (server->graphical_clients) {
            free(server->graphical_clients->client);
            free(server->graphical_clients);
        }
        free(server->parsed_info);
        free(server);
    }
}

// Helper pour libérer le client de test
static void free_test_client(client_t *client)
{
    if (client) {
        free(client->player);
        free(client);
    }
}

// =============================================================================
// TESTS pour command_pdr
// =============================================================================

Test(command_pdr_pex_simple, test_command_pdr_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    
    command_pdr(server, client, FOOD);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "pdr #1 0") != NULL);
    
    free_test_server(server);
    free_test_client(client);
}

Test(command_pdr_pex_simple, test_command_pdr_different_resources)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(5);
    
    // Test avec différents types de ressources
    command_pdr(server, client, LINEMATE);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "pdr #5 1") != NULL);
    
    reset_mocks();
    command_pdr(server, client, DERAUMERE);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "pdr #5 2") != NULL);
    
    free_test_server(server);
    free_test_client(client);
}

Test(command_pdr_pex_simple, test_command_pdr_null_client)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    
    command_pdr(server, NULL, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
}

Test(command_pdr_pex_simple, test_command_pdr_null_player)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    free(client->player);
    client->player = NULL;
    
    command_pdr(server, client, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(command_pdr_pex_simple, test_command_pdr_no_graphical_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    
    // Supprimer les clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    command_pdr(server, client, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// =============================================================================
// TESTS pour command_pex
// =============================================================================

Test(command_pdr_pex_simple, test_command_pex_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(10);
    
    command_pex(server, client);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "pex #10") != NULL);
    
    free_test_server(server);
    free_test_client(client);
}

Test(command_pdr_pex_simple, test_command_pex_different_client_ids)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(42);
    
    command_pex(server, client);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "pex #42") != NULL);
    
    free_test_server(server);
    free_test_client(client);
}

Test(command_pdr_pex_simple, test_command_pex_null_client)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    
    command_pex(server, NULL);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
}

Test(command_pdr_pex_simple, test_command_pex_null_player)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    free(client->player);
    client->player = NULL;
    
    command_pex(server, client);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

Test(command_pdr_pex_simple, test_command_pex_no_graphical_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(1);
    
    // Supprimer les clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    command_pex(server, client);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server(server);
    free_test_client(client);
}

// Test pour vérifier que les messages sont bien formatés
Test(command_pdr_pex_simple, test_message_formatting)
{
    reset_mocks();
    
    server_t *server = create_test_server();
    client_t *client = create_test_client(123);
    
    // Test command_pdr avec THYSTAME
    command_pdr(server, client, THYSTAME);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "pdr #123 6\n");
    
    reset_mocks();
    
    // Test command_pex
    command_pex(server, client);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "pex #123\n");
    
    free_test_server(server);
    free_test_client(client);
}