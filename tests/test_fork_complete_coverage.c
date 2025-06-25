/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_fork_complete_coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/egg.h"
#include "../src/Server/include/parsing.h"
#include "test_mocks.h"

TestSuite(fork_complete_coverage);

Test(fork_complete_coverage, test_fork_null_player)
{
    server_t server = {0};
    client_t client = {0};
    client.client_fd = 1;
    client.player = NULL;
    char *buffer[] = {"fork", NULL};
    
    fork_c(&server, &client, buffer);
    // Should handle NULL player gracefully
}

Test(fork_complete_coverage, test_fork_invalid_buffer_length)
{
    server_t server = {0};
    client_t client = {0};
    player_t player = {0};
    client.client_fd = 1;
    client.player = &player;
    char *buffer[] = {"fork", "extra_arg", NULL};
    
    fork_c(&server, &client, buffer);
    // Should handle invalid buffer length
}
