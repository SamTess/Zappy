/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_bct
*/

#include "test_mocks.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

// Mock implementations
static int mock_write_command_output_called = 0;
static char *mock_write_command_output_buffer = NULL;
static int mock_write_command_output_fd = -1;

void mock_write_command_output(int fd, char *buffer)
{
    mock_write_command_output_called++;
    mock_write_command_output_fd = fd;
    if (mock_write_command_output_buffer)
        free(mock_write_command_output_buffer);
    mock_write_command_output_buffer = strdup(buffer);
}

void setup_bct_test(void)
{
    mock_write_command_output_called = 0;
    mock_write_command_output_fd = -1;
    if (mock_write_command_output_buffer) {
        free(mock_write_command_output_buffer);
        mock_write_command_output_buffer = NULL;
    }
}

void teardown_bct_test(void)
{
    if (mock_write_command_output_buffer) {
        free(mock_write_command_output_buffer);
        mock_write_command_output_buffer = NULL;
    }
}

TestSuite(command_bct, .init = setup_bct_test, .fini = teardown_bct_test);

Test(command_bct, test_command_bct_valid_coordinates)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"bct", "2", "3", NULL};
    
    // Set up a tile with resources
    server->map[3][2].resources[FOOD] = 5;
    server->map[3][2].resources[LINEMATE] = 2;
    server->map[3][2].resources[DERAUMERE] = 1;
    server->map[3][2].resources[SIBUR] = 0;
    server->map[3][2].resources[MENDIANE] = 3;
    server->map[3][2].resources[PHIRAS] = 1;
    server->map[3][2].resources[THYSTAME] = 0;
    
    command_bct(server, client, buffer);
    
    cr_assert_eq(mock_write_command_output_called, 1);
    cr_assert_str_eq(mock_write_command_output_buffer, 
                     "bct 2 3 5 2 1 0 3 1 0\n");
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_bct, test_command_bct_invalid_parameters)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"bct", "invalid", "3", NULL};
    
    command_bct(server, client, buffer);
    
    cr_assert_eq(mock_write_command_output_called, 1);
    cr_assert_str_eq(mock_write_command_output_buffer, "sbp\n");
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_bct, test_command_bct_coordinates_out_of_bounds)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"bct", "100", "100", NULL};
    
    command_bct(server, client, buffer);
    
    cr_assert_eq(mock_write_command_output_called, 1);
    cr_assert_str_eq(mock_write_command_output_buffer, "sbp\n");
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_bct, test_command_bct_negative_coordinates)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"bct", "-1", "2", NULL};
    
    command_bct(server, client, buffer);
    
    cr_assert_eq(mock_write_command_output_called, 1);
    cr_assert_str_eq(mock_write_command_output_buffer, "sbp\n");
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_bct, test_command_bct_null_parameters)
{
    command_bct(NULL, NULL, NULL);
    cr_assert_eq(mock_write_command_output_called, 0);
}

Test(command_bct, test_command_bct_wrong_argument_count)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"bct", "2", NULL};
    
    command_bct(server, client, buffer);
    
    cr_assert_eq(mock_write_command_output_called, 1);
    cr_assert_str_eq(mock_write_command_output_buffer, "sbp\n");
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_bct, test_command_mtc_valid)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"mtc", NULL};
    
    command_mtc(server, client, buffer);
    
    // Should send bct for each tile (10x10 = 100 tiles)
    cr_assert_eq(mock_write_command_output_called, 100);
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_bct, test_command_mtc_invalid_arguments)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"mtc", "extra", NULL};
    
    command_mtc(server, client, buffer);
    
    cr_assert_eq(mock_write_command_output_called, 1);
    cr_assert_str_eq(mock_write_command_output_buffer, "sbp\n");
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_bct, test_send_bct_to_all_graphical_clients)
{
    server_t *server = create_mock_server();
    
    // Set up resources on tile (2, 3)
    server->map[3][2].resources[FOOD] = 7;
    server->map[3][2].resources[LINEMATE] = 1;
    
    send_bct_to_all_graphical_clients(server, 2, 3);
    
    // Should send to all graphical clients (we have 2 in mock)
    cr_assert_eq(mock_write_command_output_called, 2);
    
    free_mock_server(server);
}

Test(command_bct, test_send_bct_to_all_graphical_clients_null_server)
{
    send_bct_to_all_graphical_clients(NULL, 2, 3);
    cr_assert_eq(mock_write_command_output_called, 0);
}

Test(command_bct, test_send_mtc_to_all_graphical_clients)
{
    server_t *server = create_mock_server();
    
    send_mtc_to_all_graphical_clients(server);
    
    // Should send to all graphical clients for all tiles (2 clients * 100 tiles)
    cr_assert_eq(mock_write_command_output_called, 200);
    
    free_mock_server(server);
}

void test_graphical_connection(void)
{
    server_t server = {0};
    client_t client = {0};
    char buffer[] = "GRAPHIC\n";
    
    // Set up the client properly - this is what happens in the actual flow
    client.type = GRAPHICAL;  // This is set by is_valid_team_name when "GRAPHIC" is detected
    client.is_fully_connected = false;
    client.client_fd = 1;
    
    // Set up server
    server.graphical_clients = NULL;
    
    // This should set is_fully_connected to true for GRAPHICAL clients
    execute_com(&server, &client, buffer);
    
    // Verify the client is now fully connected
    cr_assert_eq(client.is_fully_connected, true, 
                "Graphical client should be fully connected after execute_com");
}