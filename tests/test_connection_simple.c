/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_connection_simple - Tests simples pour les fonctions de connection.c
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/parsing.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

TestSuite(connection_simple);

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

// Helper pour créer un parsing_info de test
static parsing_info_t *create_test_parsing_info(void)
{
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    
    info->port = 4242;
    info->width = 10;
    info->height = 10;
    info->client_nb = 5;
    info->frequence = 100;
    
    // Créer des noms d'équipe
    info->names = calloc(3, sizeof(char *));
    info->names[0] = strdup("team1");
    info->names[1] = strdup("team2");
    info->names[2] = NULL;
    
    return info;
}

// Helper pour libérer parsing_info
static void free_test_parsing_info(parsing_info_t *info)
{
    if (info) {
        if (info->names) {
            for (int i = 0; info->names[i]; i++) {
                free(info->names[i]);
            }
            free(info->names);
        }
        free(info);
    }
}

// Helper pour créer un serveur de test minimal
static server_t *create_test_server_minimal(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    server->nfds = 0;
    server->client = NULL;
    server->current_tick = 0;
    server->should_run = true;
    
    return server;
}

// Helper pour libérer le serveur de test
static void free_test_server_minimal(server_t *server)
{
    if (server) {
        // Libérer les clients
        client_t *current = server->client;
        while (current) {
            client_t *next = current->next;
            if (current->player) {
                free(current->player);
            }
            if (current->client_poll) {
                free(current->client_poll);
            }
            free(current);
            current = next;
        }
        
        if (server->parsed_info) {
            if (server->parsed_info->names) {
                for (int i = 0; server->parsed_info->names[i]; i++) {
                    free(server->parsed_info->names[i]);
                }
                free(server->parsed_info->names);
            }
            free(server->parsed_info);
        }
        
        free(server->total_resources);
        free(server->current_resources);
        free(server->serv_add);
        free(server->poll_manager);
        free(server);
    }
}

// Helper pour créer un client de test
static client_t *create_test_client_connection(int fd)
{
    client_t *client = calloc(1, sizeof(client_t));
    
    client->client_fd = fd;
    client->client_id = fd; // Pour simplifier
    client->next = NULL;
    client->player = calloc(1, sizeof(player_t));
    client->client_poll = calloc(1, sizeof(struct pollfd));
    client->client_poll->fd = fd;
    client->is_fully_connected = false;
    
    return client;
}

// =============================================================================
// TESTS pour init_server_resources
// =============================================================================

Test(connection_simple, test_init_server_resources_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    
    init_server_resources(server);
    
    cr_assert_not_null(server->total_resources);
    cr_assert_not_null(server->current_resources);
    
    // Vérifier que les ressources sont initialisées à 0
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(server->total_resources[i], 0);
        cr_assert_eq(server->current_resources[i], 0);
    }
    
    free_test_server_minimal(server);
}

// =============================================================================
// TESTS pour add_fd
// =============================================================================

Test(connection_simple, test_add_fd_first_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    int test_fd = 42;
    
    add_fd(server, test_fd);
    
    cr_assert_not_null(server->client);
    cr_assert_eq(server->client->client_fd, test_fd);
    cr_assert_eq(server->client->client_id, -1); // Premier client a ID -1
    
    free_test_server_minimal(server);
}

Test(connection_simple, test_add_fd_second_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    
    // Ajouter le premier client
    add_fd(server, 10);
    
    // Ajouter le deuxième client
    add_fd(server, 20);
    
    cr_assert_not_null(server->client);
    cr_assert_not_null(server->client->next);
    cr_assert_eq(server->client->next->client_fd, 20);
    cr_assert_eq(server->client->next->client_id, 0); // Deuxième client a ID 0
    
    free_test_server_minimal(server);
}

Test(connection_simple, test_add_fd_multiple_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    
    // Ajouter plusieurs clients
    add_fd(server, 10);
    add_fd(server, 20);
    add_fd(server, 30);
    
    // Vérifier la structure de la liste
    cr_assert_not_null(server->client);
    cr_assert_not_null(server->client->next);
    cr_assert_not_null(server->client->next->next);
    
    // Vérifier les IDs
    cr_assert_eq(server->client->client_id, -1);
    cr_assert_eq(server->client->next->client_id, 0);
    cr_assert_eq(server->client->next->next->client_id, 1);
    
    free_test_server_minimal(server);
}

// =============================================================================
// TESTS pour remove_fd
// =============================================================================

Test(connection_simple, test_remove_fd_single_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    client_t *client = create_test_client_connection(42);
    
    server->client = client;
    server->nfds = 1;
    
    remove_fd(server, 42);
    
    // Le client devrait être supprimé
    cr_assert_null(server->client);
    cr_assert_eq(server->nfds, 0);
    
    free_test_server_minimal(server);
}

Test(connection_simple, test_remove_fd_head_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    client_t *client1 = create_test_client_connection(10);
    client_t *client2 = create_test_client_connection(20);
    
    client1->next = client2;
    server->client = client1;
    server->nfds = 2;
    
    remove_fd(server, 10);
    
    // Le premier client devrait être supprimé, le deuxième devient la tête
    cr_assert_not_null(server->client);
    cr_assert_eq(server->client->client_fd, 20);
    cr_assert_eq(server->nfds, 1);
    
    free_test_server_minimal(server);
}

Test(connection_simple, test_remove_fd_middle_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    client_t *client1 = create_test_client_connection(10);
    client_t *client2 = create_test_client_connection(20);
    client_t *client3 = create_test_client_connection(30);
    
    client1->next = client2;
    client2->next = client3;
    server->client = client1;
    server->nfds = 3;
    
    remove_fd(server, 20);
    
    // Le client du milieu devrait être supprimé
    cr_assert_not_null(server->client);
    cr_assert_eq(server->client->client_fd, 10);
    cr_assert_not_null(server->client->next);
    cr_assert_eq(server->client->next->client_fd, 30);
    cr_assert_eq(server->nfds, 2);
    
    free_test_server_minimal(server);
}

Test(connection_simple, test_remove_fd_nonexistent)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    client_t *client = create_test_client_connection(42);
    
    server->client = client;
    server->nfds = 1;
    
    // Essayer de supprimer un client qui n'existe pas
    remove_fd(server, 999);
    
    // Le client existant devrait toujours être là
    cr_assert_not_null(server->client);
    cr_assert_eq(server->client->client_fd, 42);
    cr_assert_eq(server->nfds, 1);
    
    free_test_server_minimal(server);
}

Test(connection_simple, test_remove_fd_empty_list)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    
    // Liste vide
    server->client = NULL;
    server->nfds = 0;
    
    remove_fd(server, 42);
    
    // Devrait rester vide
    cr_assert_null(server->client);
    cr_assert_eq(server->nfds, 0);
    
    free_test_server_minimal(server);
}

// =============================================================================
// TESTS pour create_server
// =============================================================================

Test(connection_simple, test_create_server_basic)
{
    reset_mocks();
    
    server_t server;
    parsing_info_t *info = create_test_parsing_info();
    
    // Note: create_server utilise des sockets réseau, on teste la partie initialisation
    memset(&server, 0, sizeof(server_t));
    
    // Simuler l'initialisation sans la partie réseau
    server.nfds = 0;
    server.client = NULL;
    server.current_tick = 0;
    server.should_run = true;
    
    // Tester init_server_resources séparément
    init_server_resources(&server);
    
    cr_assert_not_null(server.total_resources);
    cr_assert_not_null(server.current_resources);
    
    // Nettoyer
    free(server.total_resources);
    free(server.current_resources);
    free_test_parsing_info(info);
}

Test(connection_simple, test_create_server_null_parsing_info)
{
    reset_mocks();
    
    server_t server;
    
    // Test avec parsing_info NULL (ne devrait pas planter create_server mais pourrait échouer)
    memset(&server, 0, sizeof(server_t));
    
    // On teste juste que init_server_resources fonctionne
    init_server_resources(&server);
    
    cr_assert_not_null(server.total_resources);
    cr_assert_not_null(server.current_resources);
    
    free(server.total_resources);
    free(server.current_resources);
}

// Test de gestion des ressources
Test(connection_simple, test_resource_initialization_values)
{
    reset_mocks();
    
    server_t *server = create_test_server_minimal();
    
    init_server_resources(server);
    
    // Vérifier que COUNT ressources sont initialisées
    for (int i = 0; i < COUNT; i++) {
        cr_assert_eq(server->total_resources[i], 0);
        cr_assert_eq(server->current_resources[i], 0);
    }
    
    // Modifier quelques valeurs pour tester
    server->total_resources[0] = 100;
    server->current_resources[0] = 50;
    
    cr_assert_eq(server->total_resources[0], 100);
    cr_assert_eq(server->current_resources[0], 50);
    
    free_test_server_minimal(server);
}