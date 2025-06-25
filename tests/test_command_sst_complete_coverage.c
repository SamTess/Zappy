/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_sst_complete_coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "test_mocks.h"

TestSuite(command_sst_complete_coverage);

Test(command_sst_complete_coverage, test_command_sst_null_buffer)
{
    server_t server = {0};
    client_t client = {0};
    
    command_sst(&server, &client, NULL);
    // Should handle NULL buffer gracefully
}

Test(command_sst_complete_coverage, test_command_sst_non_graphical_client)
{
    server_t server = {0};
    client_t client = {0};
    char *buffer[] = {"sst", "42", NULL};
    
    client.type = AI; // Not GRAPHICAL
    client.client_fd = 1;
    
    command_sst(&server, &client, buffer);
    // Should send "sbp" for non-graphical client
}

Test(command_sst_complete_coverage, test_command_sst_no_graphical_clients)
{
    server_t server = {0};
    client_t client = {0};
    char *buffer[] = {"sst", "42", NULL};
    
    client.type = GRAPHICAL;
    client.client_fd = 1;
    server.graphical_clients = NULL;
    
    command_sst(&server, &client, buffer);
    // Should send "sbp" when no graphical clients
}

Test(command_sst_complete_coverage, test_command_sst_wrong_buffer_length)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    char *buffer[] = {"sst", NULL}; // Only 1 argument instead of 2
    
    client.type = GRAPHICAL;
    client.client_fd = 1;
    server.graphical_clients = &graphical_client;
    
    command_sst(&server, &client, buffer);
    // Should send "sbp" for wrong argument count
}

Test(command_sst_complete_coverage, test_command_sst_invalid_time_negative)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    char *buffer[] = {"sst", "-5", NULL};
    
    client.type = GRAPHICAL;
    client.client_fd = 1;
    server.graphical_clients = &graphical_client;
    
    command_sst(&server, &client, buffer);
    // Should send "sbp" for negative time
}

Test(command_sst_complete_coverage, test_command_sst_invalid_time_zero)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    char *buffer[] = {"sst", "0", NULL};
    
    client.type = GRAPHICAL;
    client.client_fd = 1;
    server.graphical_clients = &graphical_client;
    
    command_sst(&server, &client, buffer);
    // Should send "sbp" for zero time
}

Test(command_sst_complete_coverage, test_command_sst_invalid_time_format)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    char *buffer[] = {"sst", "abc", NULL};
    
    client.type = GRAPHICAL;
    client.client_fd = 1;
    server.graphical_clients = &graphical_client;
    
    command_sst(&server, &client, buffer);
    // Should send "sbp" for invalid format
}

Test(command_sst_complete_coverage, test_command_sst_successful)
{
    server_t server = {0};
    client_t client = {0};
    client_t graphical_socket1 = {0};
    client_t graphical_socket2 = {0};
    graphical_client_t graphical_client1 = {0};
    graphical_client_t graphical_client2 = {0};
    parsing_info_t parsed_info = {0};
    char *buffer[] = {"sst", "100", NULL};
    
    // Setup client
    client.type = GRAPHICAL;
    client.client_fd = 1;
    
    // Setup graphical clients
    graphical_socket1.client_fd = 10;
    graphical_socket2.client_fd = 11;
    graphical_client1.client = &graphical_socket1;
    graphical_client1.next = &graphical_client2;
    graphical_client2.client = &graphical_socket2;
    graphical_client2.next = NULL;
    
    // Setup server
    server.graphical_clients = &graphical_client1;
    server.parsed_info = &parsed_info;
    
    command_sst(&server, &client, buffer);
    
    // Should set frequency and send to all graphical clients
    cr_assert_eq(server.parsed_info->frequence, 100);
}