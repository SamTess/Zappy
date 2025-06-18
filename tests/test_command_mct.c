/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_command_mct
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

void setup_mct_test(void)
{
    mock_write_command_output_called = 0;
    mock_write_command_output_fd = -1;
    if (mock_write_command_output_buffer) {
        free(mock_write_command_output_buffer);
        mock_write_command_output_buffer = NULL;
    }
}

void teardown_mct_test(void)
{
    if (mock_write_command_output_buffer) {
        free(mock_write_command_output_buffer);
        mock_write_command_output_buffer = NULL;
    }
}

TestSuite(command_mct, .init = setup_mct_test, .fini = teardown_mct_test);

Test(command_mct, test_command_mct_valid)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"mct", NULL};
    
    command_mct(server, client, buffer);
    
    // Should send mct for each tile (10x10 = 100 tiles)
    cr_assert_eq(mock_write_command_output_called, 100);
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_mct, test_command_mct_invalid_arguments)
{
    server_t *server = create_mock_server();
    client_t *client = create_mock_client();
    char *buffer[] = {"mct", "extra", NULL};
    
    command_mct(server, client, buffer);
    
    cr_assert_eq(mock_write_command_output_called, 1);
    cr_assert_str_eq(mock_write_command_output_buffer, "sbp\n");
    
    free_mock_server(server);
    free_mock_client(client);
}

Test(command_mct, test_command_mct_null_parameters)
{
    command_mct(NULL, NULL, NULL);
    cr_assert_eq(mock_write_command_output_called, 0);
}

Test(command_mct, test_send_mct_to_all_graphical_clients)
{
    server_t *server = create_mock_server();
    
    send_mct_to_all_graphical_clients(server);
    
    // Should send to all graphical clients for all tiles (2 clients * 100 tiles)
    cr_assert_eq(mock_write_command_output_called, 200);
    
    free_mock_server(server);
}

Test(command_mct, test_send_mct_to_all_graphical_clients_null_server)
{
    send_mct_to_all_graphical_clients(NULL);
    cr_assert_eq(mock_write_command_output_called, 0);
}