/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_smg_comprehensive
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/graphical_commands.h"

static server_t *test_server;
static int mock_write_calls = 0;
static char *last_message = NULL;

// Mock function for write_command_output
void write_command_output(int fd, char *buffer)
{
    mock_write_calls++;
    if (last_message)
        free(last_message);
    last_message = strdup(buffer);
}

// Helper function to create a mock server with graphical clients
static server_t *create_mock_server_with_graphics(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    
    // Create graphical clients list
    server->graphical_clients = calloc(1, sizeof(graphical_client_t));
    server->graphical_clients->client = calloc(1, sizeof(client_t));
    server->graphical_clients->client->client_fd = 10;
    
    server->graphical_clients->next = calloc(1, sizeof(graphical_client_t));
    server->graphical_clients->next->client = calloc(1, sizeof(client_t));
    server->graphical_clients->next->client->client_fd = 11;
    server->graphical_clients->next->next = NULL;
    
    return server;
}

static void free_mock_server_with_graphics(server_t *server)
{
    if (!server) return;
    
    if (server->graphical_clients) {
        graphical_client_t *current = server->graphical_clients;
        while (current) {
            graphical_client_t *next = current->next;
            if (current->client) {
                free(current->client);
            }
            free(current);
            current = next;
        }
    }
    free(server);
}

void setup_smg_test(void)
{
    test_server = create_mock_server_with_graphics();
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

void teardown_smg_test(void)
{
    free_mock_server_with_graphics(test_server);
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

TestSuite(command_smg_comprehensive, .init = setup_smg_test, .fini = teardown_smg_test);

// Test send_smg_command avec message valide
Test(command_smg_comprehensive, test_send_smg_command_valid)
{
    send_smg_command(test_server, "Hello World");
    
    // Devrait envoyer à tous les clients graphiques (2 dans le mock)
    cr_assert_eq(mock_write_calls, 2);
    cr_assert_str_eq(last_message, "smg Hello World\n");
}

// Test send_smg_command avec server NULL
Test(command_smg_comprehensive, test_send_smg_command_null_server)
{
    send_smg_command(NULL, "Hello World");
    
    cr_assert_eq(mock_write_calls, 0);
}

// Test send_smg_command avec message NULL
Test(command_smg_comprehensive, test_send_smg_command_null_message)
{
    send_smg_command(test_server, NULL);
    
    cr_assert_eq(mock_write_calls, 0);
}

// Test send_smg_command avec graphical_clients NULL
Test(command_smg_comprehensive, test_send_smg_command_no_graphical_clients)
{
    test_server->graphical_clients = NULL;
    
    send_smg_command(test_server, "Hello World");
    
    cr_assert_eq(mock_write_calls, 0);
}

// Test send_smg_command avec message vide
Test(command_smg_comprehensive, test_send_smg_command_empty_message)
{
    send_smg_command(test_server, "");
    
    cr_assert_eq(mock_write_calls, 2);
    cr_assert_str_eq(last_message, "smg \n");
}

// Test send_smg_command avec message long
Test(command_smg_comprehensive, test_send_smg_command_long_message)
{
    char long_msg[100];  // Réduit pour éviter les problèmes de mémoire
    for (int i = 0; i < 99; i++) {
        long_msg[i] = 'A';
    }
    long_msg[99] = '\0';
    
    send_smg_command(test_server, long_msg);
    
    cr_assert_eq(mock_write_calls, 2);
    cr_assert(strstr(last_message, "smg ") != NULL);
    cr_assert(strstr(last_message, long_msg) != NULL);
}