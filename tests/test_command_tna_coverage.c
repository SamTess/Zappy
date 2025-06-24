/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_tna_coverage - comprehensive tests for tna command
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
extern void command_tna(server_t *server, client_t *client, char **buffer);

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
static server_t *create_test_server_with_teams(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    // Create team names array
    server->team_names = calloc(4, sizeof(char *));
    server->team_names[0] = strdup("TeamA");
    server->team_names[1] = strdup("TeamB");
    server->team_names[2] = strdup("TeamC");
    server->team_names[3] = NULL;  // NULL terminated
    
    return server;
}

static client_t *create_test_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = 10;
    return client;
}

static void cleanup_test_server(server_t *server)
{
    if (server && server->team_names) {
        for (int i = 0; server->team_names[i]; i++) {
            free(server->team_names[i]);
        }
        free(server->team_names);
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

TestSuite(command_tna_coverage, .init = setup, .fini = teardown);

Test(command_tna_coverage, test_valid_tna_command)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client();
    char **buffer = calloc(2, sizeof(char *));
    buffer[0] = strdup("tna");
    buffer[1] = NULL;
    
    command_tna(server, client, buffer);
    
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "tna TeamA\n", "Should send first team name");
    cr_assert_eq(last_fd, 10, "Should send to correct client");
    
    for (int i = 0; buffer[i]; i++) {
        free(buffer[i]);
    }
    free(buffer);
    cleanup_test_server(server);
    free(client);
}

Test(command_tna_coverage, test_null_server)
{
    client_t *client = create_test_client();
    char **buffer = calloc(2, sizeof(char *));
    buffer[0] = strdup("tna");
    buffer[1] = NULL;
    
    command_tna(NULL, client, buffer);
    
    cr_assert_not_null(last_output, "Should send error for NULL server");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    for (int i = 0; buffer[i]; i++) {
        free(buffer[i]);
    }
    free(buffer);
    free(client);
}

Test(command_tna_coverage, test_null_client)
{
    server_t *server = create_test_server_with_teams();
    char **buffer = calloc(2, sizeof(char *));
    buffer[0] = strdup("tna");
    buffer[1] = NULL;
    
    command_tna(server, NULL, buffer);
    
    cr_assert_not_null(last_output, "Should send error for NULL client");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    for (int i = 0; buffer[i]; i++) {
        free(buffer[i]);
    }
    free(buffer);
    cleanup_test_server(server);
}

Test(command_tna_coverage, test_null_buffer)
{
    server_t *server = create_test_server_with_teams();
    client_t *client = create_test_client();
    
    command_tna(server, client, NULL);
    
    cr_assert_not_null(last_output, "Should send error for NULL buffer");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    cleanup_test_server(server);
    free(client);
}

Test(command_tna_coverage, test_server_without_team_names)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->team_names = NULL;
    client_t *client = create_test_client();
    char **buffer = calloc(2, sizeof(char *));
    buffer[0] = strdup("tna");
    buffer[1] = NULL;
    
    command_tna(server, client, buffer);
    
    cr_assert_not_null(last_output, "Should send error without team names");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    for (int i = 0; buffer[i]; i++) {
        free(buffer[i]);
    }
    free(buffer);
    free(server);
    free(client);
}

Test(command_tna_coverage, test_empty_team_names)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->team_names = calloc(1, sizeof(char *));
    server->team_names[0] = NULL;  // Empty array
    client_t *client = create_test_client();
    char **buffer = calloc(2, sizeof(char *));
    buffer[0] = strdup("tna");
    buffer[1] = NULL;
    
    command_tna(server, client, buffer);
    
    cr_assert_not_null(last_output, "Should send error for empty team names");
    cr_assert_str_eq(last_output, "sbp\n", "Should send sbp error");
    
    for (int i = 0; buffer[i]; i++) {
        free(buffer[i]);
    }
    free(buffer);
    free(server->team_names);
    free(server);
    free(client);
}

Test(command_tna_coverage, test_single_team)
{
    server_t *server = calloc(1, sizeof(server_t));
    server->team_names = calloc(2, sizeof(char *));
    server->team_names[0] = strdup("OnlyTeam");
    server->team_names[1] = NULL;
    
    client_t *client = create_test_client();
    char **buffer = calloc(2, sizeof(char *));
    buffer[0] = strdup("tna");
    buffer[1] = NULL;
    
    command_tna(server, client, buffer);
    
    cr_assert_not_null(last_output, "Should have sent output");
    cr_assert_str_eq(last_output, "tna OnlyTeam\n", "Should send only team name");
    
    for (int i = 0; buffer[i]; i++) {
        free(buffer[i]);
    }
    free(buffer);
    cleanup_test_server(server);
    free(client);
}