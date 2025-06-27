/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pdi_coverage - comprehensive tests for pdi command
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/player.h"
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern void command_pdi(server_t *server, client_t *client);

// Mock write_command_output
static char *last_output = NULL;
static int last_fd = -1;

void __wrap_write_command_output(int fd, const char *output)
{
    last_fd = fd;
    if (last_output) {
        free(last_output);
    }
    last_output = strdup(output);
}

// Helper functions
static server_t *create_test_server_with_graphics(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    graphical_client_t *graph_client = calloc(1, sizeof(graphical_client_t));
    client_t *client = calloc(1, sizeof(client_t));
    
    client->client_fd = 42;
    client->client_id = 1;
    graph_client->client = client;
    
    server->graphical_clients = graph_client;
    
    return server;
}

static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_id = 123;
    client->player = player;
    
    return client;
}

static void cleanup_test_server(server_t *server)
{
    if (server && server->graphical_clients) {
        if (server->graphical_clients->client) {
            free(server->graphical_clients->client);
        }
        free(server->graphical_clients);
    }
    free(server);
}

static void cleanup_test_client(client_t *client)
{
    if (client) {
        free(client->player);
        free(client);
    }
}

void setup(void)
{
    last_output = NULL;
    last_fd = -1;
}

void teardown(void)
{
    if (last_output) {
        free(last_output);
        last_output = NULL;
    }
}

TestSuite(command_pdi_coverage, .init = setup, .fini = teardown);

Test(command_pdi_coverage, test_valid_pdi_command)
{
    server_t *server = create_test_server_with_graphics();
    client_t *client = create_test_client();
    
    command_pdi(server, client);
    
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "pdi #123\n", "Should send correct pdi message");
    cr_assert_eq(last_fd, 42, "Should send to correct fd");
    
    cleanup_test_server(server);
    cleanup_test_client(client);
}

Test(command_pdi_coverage, test_null_client)
{
    server_t *server = create_test_server_with_graphics();
    
    command_pdi(server, NULL);
    
    cr_assert_null(last_output, "Should not send output for NULL client");
    
    cleanup_test_server(server);
}

Test(command_pdi_coverage, test_null_server)
{
    client_t *client = create_test_client();
    
    command_pdi(NULL, client);
    
    cr_assert_null(last_output, "Should not send output for NULL server");
    
    cleanup_test_client(client);
}

Test(command_pdi_coverage, test_client_without_player)
{
    server_t *server = create_test_server_with_graphics();
    client_t *client = calloc(1, sizeof(client_t));
    client->client_id = 123;
    client->player = NULL;
    
    command_pdi(server, client);
    
    cr_assert_null(last_output, "Should not send output for client without player");
    
    cleanup_test_server(server);
    free(client);
}

Test(command_pdi_coverage, test_server_without_graphical_clients)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->graphical_clients = NULL;
    client_t *client = create_test_client();
    
    command_pdi(server, client);
    
    cr_assert_null(last_output, "Should not send output without graphical clients");
    
    free(server);
    cleanup_test_client(client);
}

Test(command_pdi_coverage, test_multiple_graphical_clients)
{
    server_t *server = create_test_server_with_graphics();
    
    // Add second graphical client
    graphical_client_t *second_graph = calloc(1, sizeof(graphical_client_t));
    client_t *second_client = calloc(1, sizeof(client_t));
    second_client->client_fd = 43;
    second_graph->client = second_client;
    server->graphical_clients->next = second_graph;
    
    client_t *client = create_test_client();
    
    command_pdi(server, client);
    
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "pdi #123\n", "Should send correct pdi message");
    
    // Cleanup
    free(second_graph->client);
    free(second_graph);
    cleanup_test_server(server);
    cleanup_test_client(client);
}