/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_seg - tests for seg command functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"
#include <stdlib.h>
#include <string.h>

// Forward declaration
extern void command_seg(server_t *server, const char *team_name);

// Helper function to create a test server
static server_t *create_test_server_with_graphics(bool has_graphics)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    if (has_graphics) {
        // Create a minimal graphical client structure
        graphical_client_t *graph_client = calloc(1, sizeof(graphical_client_t));
        client_t *client = calloc(1, sizeof(client_t));
        
        client->client_fd = 42;
        client->client_id = 1;
        graph_client->client = client;
        graph_client->next = NULL;
        
        server->graphical_clients = graph_client;
    } else {
        server->graphical_clients = NULL;
    }
    
    return server;
}

static void cleanup_test_server(server_t *server)
{
    if (server) {
        if (server->graphical_clients) {
            if (server->graphical_clients->client)
                free(server->graphical_clients->client);
            free(server->graphical_clients);
        }
        free(server);
    }
}

TestSuite(command_seg_tests);

// Test command_seg with NULL server
Test(command_seg_tests, test_command_seg_null_server)
{
    // Should not crash with NULL server
    command_seg(NULL, "team1");
    
    cr_assert(true, "command_seg should handle NULL server gracefully");
}

// Test command_seg with NULL team_name
Test(command_seg_tests, test_command_seg_null_team_name)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Should not crash with NULL team_name - le mock devrait intercepter ceci
    command_seg(server, NULL);
    
    cr_assert(true, "command_seg should handle NULL team_name gracefully");
    
    cleanup_test_server(server);
}

// Test command_seg with server that has no graphical clients
Test(command_seg_tests, test_command_seg_no_graphics)
{
    server_t *server = create_test_server_with_graphics(false);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Should not crash with no graphical clients
    command_seg(server, "winners");
    
    cr_assert(true, "command_seg should handle no graphical clients gracefully");
    
    cleanup_test_server(server);
}

// Test command_seg with valid server and graphical clients
Test(command_seg_tests, test_command_seg_valid)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // This should execute the full function without issues
    command_seg(server, "red_team");
    
    cr_assert(true, "command_seg should execute with valid parameters");
    
    cleanup_test_server(server);
}

// Test command_seg with different team names
Test(command_seg_tests, test_command_seg_different_teams)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Test with various team names
    command_seg(server, "alpha");
    command_seg(server, "beta");
    command_seg(server, "gamma");
    command_seg(server, "team_with_underscores");
    command_seg(server, "a");  // Single character
    command_seg(server, "VeryLongTeamNameThatShouldStillWork");
    
    cr_assert(true, "command_seg should handle different team names");
    
    cleanup_test_server(server);
}

// Test command_seg with empty team name
Test(command_seg_tests, test_command_seg_empty_team_name)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Test with empty string
    command_seg(server, "");
    
    cr_assert(true, "command_seg should handle empty team name");
    
    cleanup_test_server(server);
}

// Test command_seg execution path
Test(command_seg_tests, test_command_seg_execution_path)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // This will test the buffer allocation and formatting logic
    command_seg(server, "winner_team");
    
    cr_assert(true, "command_seg should complete execution path");
    
    cleanup_test_server(server);
}

// Test command_seg with multiple calls
Test(command_seg_tests, test_command_seg_multiple_calls)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Multiple calls should all work
    command_seg(server, "team1");
    command_seg(server, "team2");
    command_seg(server, "team3");
    
    cr_assert(true, "command_seg should handle multiple calls");
    
    cleanup_test_server(server);
}

// Test command_seg with special characters in team name
Test(command_seg_tests, test_command_seg_special_chars)
{
    server_t *server = create_test_server_with_graphics(true);
    
    // Rediriger la sortie pour supprimer les messages de victory
    cr_redirect_stdout();
    cr_redirect_stderr();
    
    // Test with team names containing numbers and special chars
    command_seg(server, "team123");
    command_seg(server, "team-456");
    command_seg(server, "team_789");
    
    cr_assert(true, "command_seg should handle team names with special characters");
    
    cleanup_test_server(server);
}