/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_sst_coverage - comprehensive tests for sst command
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/graphical_commands.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations
extern void command_sst(server_t *server, client_t *client, char **buffer);
extern int arr_len(char **arr);

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

// Mock arr_len function
int __wrap_arr_len(char **arr)
{
    if (!arr) return 0;
    int count = 0;
    while (arr[count]) count++;
    return count;
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
    server->tick_delay = 100;  // Default tick delay
    
    return server;
}

static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = 10;
    return client;
}

static char **create_test_buffer(int argc, ...)
{
    va_list args;
    char **buffer = calloc(argc + 1, sizeof(char *));
    
    va_start(args, argc);
    for (int i = 0; i < argc; i++) {
        char *arg = va_arg(args, char *);
        buffer[i] = strdup(arg);
    }
    va_end(args);
    
    return buffer;
}

static void cleanup_test_buffer(char **buffer)
{
    if (!buffer) return;
    for (int i = 0; buffer[i]; i++) {
        free(buffer[i]);
    }
    free(buffer);
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
}

void teardown(void)
{
    if (last_output) {
        free(last_output);
        last_output = NULL;
    }
}

TestSuite(command_sst_coverage, .init = setup, .fini = teardown);

Test(command_sst_coverage, test_valid_sst_command)
{
    server_t *server = create_test_server_with_graphics();
    client_t *client = create_test_client();
    char **buffer = create_test_buffer(2, "sst", "50");
    
    command_sst(server, client, buffer);
    
    cr_assert_eq(server->tick_delay, 50, "Should update tick delay");
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "sst 50\n", "Should send correct sst response");
    cr_assert_eq(last_fd, 42, "Should send to graphical client");
    
    cleanup_test_buffer(buffer);
    cleanup_test_server(server);
    free(client);
}

Test(command_sst_coverage, test_null_server)
{
    client_t *client = create_test_client();
    char **buffer = create_test_buffer(2, "sst", "50");
    
    command_sst(NULL, client, buffer);
    
    cr_assert_not_null(last_output, "Should send error for NULL server");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    cleanup_test_buffer(buffer);
    free(client);
}

Test(command_sst_coverage, test_null_client)
{
    server_t *server = create_test_server_with_graphics();
    char **buffer = create_test_buffer(2, "sst", "50");
    
    command_sst(server, NULL, buffer);
    
    cr_assert_not_null(last_output, "Should send error for NULL client");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    cleanup_test_buffer(buffer);
    cleanup_test_server(server);
}

Test(command_sst_coverage, test_null_buffer)
{
    server_t *server = create_test_server_with_graphics();
    client_t *client = create_test_client();
    
    command_sst(server, client, NULL);
    
    cr_assert_not_null(last_output, "Should send error for NULL buffer");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    cleanup_test_server(server);
    free(client);
}

Test(command_sst_coverage, test_invalid_buffer_length)
{
    server_t *server = create_test_server_with_graphics();
    client_t *client = create_test_client();
    char **buffer = create_test_buffer(1, "sst");  // Missing time parameter
    
    command_sst(server, client, buffer);
    
    cr_assert_not_null(last_output, "Should send error for invalid buffer length");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    cleanup_test_buffer(buffer);
    cleanup_test_server(server);
    free(client);
}

Test(command_sst_coverage, test_server_without_graphical_clients)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->graphical_clients = NULL;
    server->tick_delay = 100;
    client_t *client = create_test_client();
    char **buffer = create_test_buffer(2, "sst", "25");
    
    command_sst(server, client, buffer);
    
    cr_assert_not_null(last_output, "Should send error without graphical clients");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    cleanup_test_buffer(buffer);
    free(server);
    free(client);
}

Test(command_sst_coverage, test_zero_tick_delay)
{
    server_t *server = create_test_server_with_graphics();
    client_t *client = create_test_client();
    char **buffer = create_test_buffer(2, "sst", "0");
    
    command_sst(server, client, buffer);
    
    cr_assert_eq(server->tick_delay, 0, "Should accept zero tick delay");
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "sst 0\n", "Should send correct sst response");
    
    cleanup_test_buffer(buffer);
    cleanup_test_server(server);
    free(client);
}

Test(command_sst_coverage, test_large_tick_delay)
{
    server_t *server = create_test_server_with_graphics();
    client_t *client = create_test_client();
    char **buffer = create_test_buffer(2, "sst", "9999");
    
    command_sst(server, client, buffer);
    
    cr_assert_eq(server->tick_delay, 9999, "Should accept large tick delay");
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "sst 9999\n", "Should send correct sst response");
    
    cleanup_test_buffer(buffer);
    cleanup_test_server(server);
    free(client);
}