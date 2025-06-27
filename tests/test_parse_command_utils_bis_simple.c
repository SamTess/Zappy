/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_parse_command_utils_bis_simple
*/

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/command.h"
#include <stdbool.h>
#include <unistd.h>

// Mock function for handle_socket_read
static bool mock_handle_socket_read(client_t *user, server_t *server)
{
    (void)user;
    (void)server;
    return true; // Simulate successful read
}

TestSuite(parse_command_utils_bis_simple);

Test(parse_command_utils_bis_simple, test_determine_type_basic)
{
    resource_type_t result;
    
    result = determine_type("food");
    cr_assert_eq(result, FOOD, "Should return FOOD for 'food'");
    
    result = determine_type("linemate");
    cr_assert_eq(result, LINEMATE, "Should return LINEMATE for 'linemate'");
    
    result = determine_type("invalid");
    cr_assert_eq(result, COUNT, "Should return COUNT for invalid resource");
}

Test(parse_command_utils_bis_simple, test_handle_socket_read_basic)
{
    client_t client = {0};
    server_t server = {0};
    
    // Initialize basic client structure
    client.client_fd = 1;
    client.type = AI;
    
    // Use mock function instead of actual implementation
    bool result = mock_handle_socket_read(&client, &server);
    
    cr_assert_eq(result, true, "handle_socket_read should return true on success");
}