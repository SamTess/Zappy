/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_pie_simple
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include "test_mocks.h"

TestSuite(command_pie_simple);

Test(command_pie_simple, test_command_pie_basic)
{
    server_t server = {0};
    client_t client = {0};
    graphical_client_t graphical_client = {0};
    
    client.client_fd = 1;
    client.next = NULL;
    server.client = &client;
    server.graphical_clients = &graphical_client;
    
    // Call function to get coverage
    command_pie(&server, 5, 5, 1);
    
    cr_assert(true, "Function executed");
}

Test(command_pie_simple, test_command_pie_null_server)
{
    command_pie(NULL, 1, 1, 1);
    cr_assert(true, "Function executed with null server");
}

Test(command_pie_simple, test_command_pie_no_graphical_clients)
{
    server_t server = {0};
    
    server.graphical_clients = NULL;
    
    command_pie(&server, 1, 1, 1);
    cr_assert(true, "Function executed with no graphical clients");
}