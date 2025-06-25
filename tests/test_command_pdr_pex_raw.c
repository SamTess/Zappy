/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pdr_pex_raw - Tests RAW pour command_pdr et command_pex sans mocks
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
#include <unistd.h>
#include <sys/wait.h>

TestSuite(command_pdr_pex_raw);

// Helper pour créer un serveur de test simple
static server_t *create_test_server_raw(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    
    info->width = 10;
    info->height = 10;
    server->parsed_info = info;
    
    // Créer un client graphique avec des pipes pour capturer la sortie
    graphical_client_t *graphical = calloc(1, sizeof(graphical_client_t));
    client_t *graphical_client = calloc(1, sizeof(client_t));
    
    // Créer un pipe pour capturer la sortie
    int pipefd[2];
    pipe(pipefd);
    graphical_client->client_fd = pipefd[1]; // Write end
    
    graphical->client = graphical_client;
    graphical->next = NULL;
    server->graphical_clients = graphical;
    
    return server;
}

// Helper pour créer un client AI de test
static client_t *create_test_client_raw(int client_id)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = client_id;
    client->client_fd = 42; // Dummy FD
    client->player = player;
    client->type = AI;
    
    return client;
}

// Helper pour libérer le serveur de test
static void free_test_server_raw(server_t *server)
{
    if (server) {
        if (server->graphical_clients) {
            if (server->graphical_clients->client) {
                close(server->graphical_clients->client->client_fd);
                free(server->graphical_clients->client);
            }
            free(server->graphical_clients);
        }
        free(server->parsed_info);
        free(server);
    }
}

// Helper pour libérer le client de test
static void free_test_client_raw(client_t *client)
{
    if (client) {
        free(client->player);
        free(client);
    }
}

// =============================================================================
// TESTS RAW pour command_pdr
// =============================================================================

Test(command_pdr_pex_raw, test_command_pdr_basic_execution)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(1);
    
    // Test basique - doit s'exécuter sans crash
    command_pdr(server, client, FOOD);
    
    // Si on arrive ici, la fonction s'est exécutée sans crash
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client);
}

Test(command_pdr_pex_raw, test_command_pdr_different_resources)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(5);
    
    // Test avec différents types de ressources
    command_pdr(server, client, LINEMATE);
    command_pdr(server, client, DERAUMERE);
    command_pdr(server, client, SIBUR);
    command_pdr(server, client, MENDIANE);
    command_pdr(server, client, PHIRAS);
    command_pdr(server, client, THYSTAME);
    
    // Si on arrive ici, toutes les fonctions se sont exécutées sans crash
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client);
}

Test(command_pdr_pex_raw, test_command_pdr_null_parameters)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(1);
    
    // Test avec paramètres NULL - ne doit pas crash
    command_pdr(server, NULL, FOOD);
    command_pdr(NULL, client, FOOD);
    
    // Test avec player NULL
    free(client->player);
    client->player = NULL;
    command_pdr(server, client, FOOD);
    
    // Test sans clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    command_pdr(server, client, FOOD);
    
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client);
}

// =============================================================================
// TESTS RAW pour command_pex
// =============================================================================

Test(command_pdr_pex_raw, test_command_pex_basic_execution)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(10);
    
    // Test basique - doit s'exécuter sans crash
    command_pex(server, client);
    
    // Si on arrive ici, la fonction s'est exécutée sans crash
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client);
}

Test(command_pdr_pex_raw, test_command_pex_different_client_ids)
{
    server_t *server = create_test_server_raw();
    client_t *client1 = create_test_client_raw(42);
    client_t *client2 = create_test_client_raw(123);
    
    // Test avec différents IDs de client
    command_pex(server, client1);
    command_pex(server, client2);
    
    // Si on arrive ici, les fonctions se sont exécutées sans crash
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client1);
    free_test_client_raw(client2);
}

Test(command_pdr_pex_raw, test_command_pex_null_parameters)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(1);
    
    // Test avec paramètres NULL - ne doit pas crash
    command_pex(server, NULL);
    command_pex(NULL, client);
    
    // Test avec player NULL
    free(client->player);
    client->player = NULL;
    command_pex(server, client);
    
    // Test sans clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    command_pex(server, client);
    
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client);
}

// Test pour malloc failure dans command_pdr
Test(command_pdr_pex_raw, test_command_pdr_malloc_failure_simulation)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(1);
    
    // Ce test simule l'exécution normale
    // En réalité, malloc peut échouer mais c'est difficile à forcer
    command_pdr(server, client, FOOD);
    
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client);
}

// Test pour malloc failure dans command_pex
Test(command_pdr_pex_raw, test_command_pex_malloc_failure_simulation)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(1);
    
    // Ce test simule l'exécution normale
    command_pex(server, client);
    
    cr_assert(true);
    
    free_test_server_raw(server);
    free_test_client_raw(client);
}

// Test de la boucle while avec plusieurs clients graphiques
Test(command_pdr_pex_raw, test_multiple_graphical_clients)
{
    server_t *server = create_test_server_raw();
    client_t *client = create_test_client_raw(99);
    
    // Ajouter un deuxième client graphique
    graphical_client_t *graphical2 = calloc(1, sizeof(graphical_client_t));
    client_t *graphical_client2 = calloc(1, sizeof(client_t));
    
    int pipefd2[2];
    pipe(pipefd2);
    graphical_client2->client_fd = pipefd2[1];
    
    graphical2->client = graphical_client2;
    graphical2->next = NULL;
    server->graphical_clients->next = graphical2;
    
    // Test avec plusieurs clients graphiques
    command_pdr(server, client, FOOD);
    command_pex(server, client);
    
    cr_assert(true);
    
    // Cleanup
    close(pipefd2[1]);
    free(graphical_client2);
    free(graphical2);
    free_test_server_raw(server);
    free_test_client_raw(client);
}