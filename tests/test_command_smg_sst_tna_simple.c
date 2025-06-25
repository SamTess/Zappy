/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_smg_sst_tna_simple - Tests simples pour command_smg, command_sst et command_tna
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

TestSuite(command_smg_sst_tna_simple);

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

// Helper pour créer un serveur de test avec parsing info complète
static server_t *create_test_server_with_teams(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    
    info->width = 10;
    info->height = 10;
    info->frequence = 100;
    
    // Créer des noms d'équipe pour les tests TNA
    info->names = calloc(4, sizeof(char *));
    info->names[0] = strdup("team1");
    info->names[1] = strdup("team2");
    info->names[2] = strdup("team3");
    info->names[3] = NULL;
    
    server->parsed_info = info;
    
    // Créer un client graphique
    graphical_client_t *graphical = calloc(1, sizeof(graphical_client_t));
    client_t *graphical_client = calloc(1, sizeof(client_t));
    graphical_client->client_fd = 42;
    graphical_client->type = GRAPHICAL;
    graphical->client = graphical_client;
    graphical->next = NULL;
    server->graphical_clients = graphical;
    
    return server;
}

// Helper pour créer un client graphique de test
static client_t *create_test_graphical_client(int client_id)
{
    client_t *client = calloc(1, sizeof(client_t));
    
    client->client_id = client_id;
    client->client_fd = 10;
    client->type = GRAPHICAL;
    
    return client;
}

// Helper pour libérer le serveur de test
static void free_test_server_with_teams(server_t *server)
{
    if (server) {
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
            free(server->graphical_clients->client);
            free(server->graphical_clients);
        }
        free(server);
    }
}

// Helper pour libérer le client de test
static void free_test_client(client_t *client)
{
    if (client) {
        free(client);
    }
}

// =============================================================================
// TESTS pour send_smg_command
// =============================================================================

Test(command_smg_sst_tna_simple, test_send_smg_command_null_server)
{
    reset_mocks();
    
    send_smg_command(NULL, "Hello");
    
    cr_assert_eq(mock_write_calls, 0);
}

Test(command_smg_sst_tna_simple, test_send_smg_command_null_message)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    
    send_smg_command(server, NULL);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_teams(server);
}

Test(command_smg_sst_tna_simple, test_send_smg_command_no_graphical_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    
    // Supprimer les clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    send_smg_command(server, "Hello");
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_teams(server);
}

// =============================================================================
// TESTS pour command_sst
// =============================================================================

Test(command_smg_sst_tna_simple, test_command_sst_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_graphical_client(1);
    char *args[] = {"sst", "50", NULL};
    
    command_sst(server, client, args);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "sst 50") != NULL);
    cr_assert_eq(server->parsed_info->frequence, 50);
    
    free_test_server_with_teams(server);
    free_test_client(client);
}

Test(command_smg_sst_tna_simple, test_command_sst_different_values)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_graphical_client(1);
    char *args[] = {"sst", "100", NULL};
    
    command_sst(server, client, args);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sst 100") != NULL);
    cr_assert_eq(server->parsed_info->frequence, 100);
    
    reset_mocks();
    char *args2[] = {"sst", "200", NULL};
    command_sst(server, client, args2);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sst 200") != NULL);
    cr_assert_eq(server->parsed_info->frequence, 200);
    
    free_test_server_with_teams(server);
    free_test_client(client);
}

Test(command_smg_sst_tna_simple, test_command_sst_invalid_arguments)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_graphical_client(1);
    
    // Test avec NULL args
    command_sst(server, client, NULL);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    reset_mocks();
    // Test avec trop peu d'arguments
    char *args1[] = {"sst", NULL};
    command_sst(server, client, args1);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    reset_mocks();
    // Test avec valeur négative
    char *args2[] = {"sst", "-10", NULL};
    command_sst(server, client, args2);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    reset_mocks();
    // Test avec valeur zéro
    char *args3[] = {"sst", "0", NULL};
    command_sst(server, client, args3);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    free_test_server_with_teams(server);
    free_test_client(client);
}

Test(command_smg_sst_tna_simple, test_command_sst_non_graphical_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_graphical_client(1);
    client->type = AI; // Client AI au lieu de GRAPHICAL
    char *args[] = {"sst", "50", NULL};
    
    command_sst(server, client, args);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    free_test_server_with_teams(server);
    free_test_client(client);
}

// =============================================================================
// TESTS pour command_tna
// =============================================================================

Test(command_smg_sst_tna_simple, test_command_tna_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_graphical_client(1);
    char *args[] = {"tna", NULL};
    
    command_tna(server, client, args);
    
    // Devrait envoyer un message pour chaque équipe (3 équipes)
    cr_assert_eq(mock_write_calls, 3);
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "tna") != NULL);
    
    free_test_server_with_teams(server);
    free_test_client(client);
}

Test(command_smg_sst_tna_simple, test_command_tna_invalid_arguments)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_graphical_client(1);
    
    // Test avec NULL args
    command_tna(server, client, NULL);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    reset_mocks();
    // Test avec trop d'arguments
    char *args[] = {"tna", "extra", NULL};
    command_tna(server, client, args);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    free_test_server_with_teams(server);
    free_test_client(client);
}

Test(command_smg_sst_tna_simple, test_command_tna_null_server)
{
    reset_mocks();
    
    client_t *client = create_test_graphical_client(1);
    char *args[] = {"tna", NULL};
    
    command_tna(NULL, client, args);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    free_test_client(client);
}

Test(command_smg_sst_tna_simple, test_command_tna_no_graphical_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_graphical_client(1);
    char *args[] = {"tna", NULL};
    
    // Supprimer les clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    command_tna(server, client, args);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "sbp") != NULL);
    
    free_test_server_with_teams(server);
    free_test_client(client);
}