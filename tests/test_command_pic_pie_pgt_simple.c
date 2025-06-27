/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pic_pie_pgt_simple - Tests simples pour command_pic, command_pie et command_pgt
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

TestSuite(command_pic_pie_pgt_simple);

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

// Helper pour créer un serveur de test avec map
static server_t *create_test_server_with_map(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *info = calloc(1, sizeof(parsing_info_t));
    
    info->width = 10;
    info->height = 10;
    server->parsed_info = info;
    
    // Créer la map
    server->map = calloc(info->height, sizeof(tile_t *));
    for (int i = 0; i < info->height; i++) {
        server->map[i] = calloc(info->width, sizeof(tile_t));
        for (int j = 0; j < info->width; j++) {
            tile_init(&server->map[i][j]);
        }
    }
    
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
static void free_test_server_with_map(server_t *server)
{
    if (server) {
        if (server->map) {
            for (int i = 0; i < server->parsed_info->height; i++) {
                for (int j = 0; j < server->parsed_info->width; j++) {
                    tile_free(&server->map[i][j]);
                }
                free(server->map[i]);
            }
            free(server->map);
        }
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
// TESTS pour command_pgt
// =============================================================================

Test(command_pic_pie_pgt_simple, test_command_pgt_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    client_t *client = create_test_client(1);
    
    command_pgt(server, client, FOOD);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "pgt #1 0") != NULL);
    
    free_test_server_with_map(server);
    free_test_client(client);
}

Test(command_pic_pie_pgt_simple, test_command_pgt_different_resources)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    client_t *client = create_test_client(5);
    
    // Test avec différents types de ressources
    command_pgt(server, client, LINEMATE);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "pgt #5 1") != NULL);
    
    reset_mocks();
    command_pgt(server, client, THYSTAME);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "pgt #5 6") != NULL);
    
    free_test_server_with_map(server);
    free_test_client(client);
}

Test(command_pic_pie_pgt_simple, test_command_pgt_null_client)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    
    command_pgt(server, NULL, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_map(server);
}

Test(command_pic_pie_pgt_simple, test_command_pgt_null_player)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    client_t *client = create_test_client(1);
    free(client->player);
    client->player = NULL;
    
    command_pgt(server, client, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_map(server);
    free_test_client(client);
}

Test(command_pic_pie_pgt_simple, test_command_pgt_no_graphical_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    client_t *client = create_test_client(1);
    
    // Supprimer les clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    command_pgt(server, client, FOOD);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_map(server);
    free_test_client(client);
}

// =============================================================================
// TESTS pour command_pic
// =============================================================================

Test(command_pic_pie_pgt_simple, test_command_pic_basic)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    
    command_pic(server, 5, 5, 1);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "pic 5 5 1") != NULL);
    
    free_test_server_with_map(server);
}

Test(command_pic_pie_pgt_simple, test_command_pic_different_coordinates)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    
    command_pic(server, 0, 0, 2);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "pic 0 0 2") != NULL);
    
    reset_mocks();
    command_pic(server, 9, 9, 3);
    cr_assert_eq(mock_write_calls, 1);
    cr_assert(strstr(last_message, "pic 9 9 3") != NULL);
    
    free_test_server_with_map(server);
}

Test(command_pic_pie_pgt_simple, test_command_pic_null_server)
{
    reset_mocks();
    
    command_pic(NULL, 5, 5, 1);
    
    cr_assert_eq(mock_write_calls, 0);
}

Test(command_pic_pie_pgt_simple, test_command_pic_no_graphical_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    
    // Supprimer les clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    command_pic(server, 5, 5, 1);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_map(server);
}

Test(command_pic_pie_pgt_simple, test_command_pic_invalid_coordinates)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    
    // Tester des coordonnées invalides
    command_pic(server, -1, 5, 1);
    cr_assert_eq(mock_write_calls, 0);
    
    command_pic(server, 5, -1, 1);
    cr_assert_eq(mock_write_calls, 0);
    
    command_pic(server, 10, 5, 1); // width = 10, donc 10 est invalide
    cr_assert_eq(mock_write_calls, 0);
    
    command_pic(server, 5, 10, 1); // height = 10, donc 10 est invalide
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_map(server);
}

// =============================================================================
// TESTS pour command_pie
// =============================================================================

Test(command_pic_pie_pgt_simple, test_command_pie_null_server)
{
    reset_mocks();
    
    command_pie(NULL, 5, 5, 1);
    
    cr_assert_eq(mock_write_calls, 0);
}

Test(command_pic_pie_pgt_simple, test_command_pie_no_graphical_clients)
{
    reset_mocks();
    
    server_t *server = create_test_server_with_map();
    
    // Supprimer les clients graphiques
    free(server->graphical_clients->client);
    free(server->graphical_clients);
    server->graphical_clients = NULL;
    
    command_pie(server, 5, 5, 1);
    
    cr_assert_eq(mock_write_calls, 0);
    
    free_test_server_with_map(server);
}