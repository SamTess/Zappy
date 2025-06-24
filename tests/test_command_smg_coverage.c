/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_smg_coverage - comprehensive tests for smg command
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern void send_smg_command(server_t *server, const char *msg);

// Mock write_command_output
static char *last_output = NULL;
static int last_fd = -1;
static int call_count = 0;

void __wrap_write_command_output(int fd, const char *output)
{
    last_fd = fd;
    call_count++;
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

void setup(void)
{
    last_output = NULL;
    last_fd = -1;
    call_count = 0;
}

void teardown(void)
{
    if (last_output) {
        free(last_output);
        last_output = NULL;
    }
}

TestSuite(command_smg_coverage, .init = setup, .fini = teardown);

Test(command_smg_coverage, test_valid_smg_command)
{
    server_t *server = create_test_server_with_graphics();
    const char *message = "Test server message";
    
    send_smg_command(server, message);
    
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "smg Test server message\n", "Should send correct smg message");
    cr_assert_eq(last_fd, 42, "Should send to correct fd");
    cr_assert_eq(call_count, 1, "Should call write_command_output once");
    
    cleanup_test_server(server);
}

Test(command_smg_coverage, test_null_server)
{
    send_smg_command(NULL, "Test message");
    
    cr_assert_null(last_output, "Should not send output for NULL server");
    cr_assert_eq(call_count, 0, "Should not call write_command_output");
}

Test(command_smg_coverage, test_null_message)
{
    server_t *server = create_test_server_with_graphics();
    
    send_smg_command(server, NULL);
    
    cr_assert_null(last_output, "Should not send output for NULL message");
    cr_assert_eq(call_count, 0, "Should not call write_command_output");
    
    cleanup_test_server(server);
}

Test(command_smg_coverage, test_server_without_graphical_clients)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->graphical_clients = NULL;
    
    send_smg_command(server, "Test message");
    
    cr_assert_null(last_output, "Should not send output without graphical clients");
    cr_assert_eq(call_count, 0, "Should not call write_command_output");
    
    free(server);
}

Test(command_smg_coverage, test_multiple_graphical_clients)
{
    server_t *server = create_test_server_with_graphics();
    
    // Add second graphical client
    graphical_client_t *second_graph = calloc(1, sizeof(graphical_client_t));
    client_t *second_client = calloc(1, sizeof(client_t));
    second_client->client_fd = 43;
    second_graph->client = second_client;
    server->graphical_clients->next = second_graph;
    
    send_smg_command(server, "Broadcast message");
    
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "smg Broadcast message\n", "Should send correct smg message");
    cr_assert_eq(call_count, 2, "Should call write_command_output twice for two clients");
    
    // Cleanup
    free(second_graph->client);
    free(second_graph);
    cleanup_test_server(server);
}

Test(command_smg_coverage, test_empty_message)
{
    server_t *server = create_test_server_with_graphics();
    
    send_smg_command(server, "");
    
    cr_assert_not_null(last_output, "Should have sent output even for empty message");
    cr_assert_str_eq(last_output, "smg \n", "Should send smg with empty message");
    cr_assert_eq(call_count, 1, "Should call write_command_output once");
    
    cleanup_test_server(server);
}

Test(command_smg_coverage, test_long_message)
{
    server_t *server = create_test_server_with_graphics();
    const char *long_msg = "This is a very long message that tests the buffer allocation and formatting";
    
    send_smg_command(server, long_msg);
    
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "smg This is a very long message that tests the buffer allocation and formatting\n", "Should send correct long smg message");
    cr_assert_eq(call_count, 1, "Should call write_command_output once");
    
    cleanup_test_server(server);
}