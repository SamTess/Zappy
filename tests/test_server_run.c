/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_server_run - Unit tests for server_run.c functions
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/pending_cmd_utils.h"
#include "test_mocks.h"

// Define cr_assert_false if not available
#ifndef cr_assert_false
#define cr_assert_false(condition) cr_assert_not(condition)
#endif

// Mock variables for server_run tests
static int mock_accept_calls = 0;
static int mock_accept_return = -1;
static int mock_add_fd_calls = 0;
static int mock_get_message_calls = 0;
static int mock_update_game_tick_calls = 0;
static int mock_poll_calls = 0;
static int mock_poll_return = 0;
static struct pollfd *mock_poll_fds = NULL;
static int mock_gettimeofday_calls = 0;
static struct timeval mock_current_time = {0, 0};

// Reset mock functions
static void reset_server_run_mocks(void)
{
    mock_accept_calls = 0;
    mock_accept_return = -1;
    mock_add_fd_calls = 0;
    mock_get_message_calls = 0;
    mock_update_game_tick_calls = 0;
    mock_poll_calls = 0;
    mock_poll_return = 0;
    mock_gettimeofday_calls = 0;
    mock_current_time.tv_sec = 0;
    mock_current_time.tv_usec = 0;
}

// Mock implementations (only the ones not already defined in test_mocks.c)
int __wrap_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    mock_accept_calls++;
    (void)sockfd;
    (void)addr;
    (void)addrlen;
    return mock_accept_return;
}

void __wrap_add_fd(server_t *server, int fd)
{
    mock_add_fd_calls++;
    (void)server;
    (void)fd;
}

void __wrap_get_message(server_t *server, client_t *client)
{
    mock_get_message_calls++;
    (void)server;
    (void)client;
}

void __wrap_update_game_tick(server_t *server)
{
    mock_update_game_tick_calls++;
    (void)server;
}

int __wrap_poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    mock_poll_calls++;
    mock_poll_fds = fds;
    (void)nfds;
    (void)timeout;
    return mock_poll_return;
}

int __wrap_gettimeofday(struct timeval *tv, struct timezone *tz)
{
    mock_gettimeofday_calls++;
    (void)tz;
    if (tv) {
        *tv = mock_current_time;
    }
    return 0;
}

void __wrap_perror(const char *s)
{
    (void)s;
    // Mock perror to avoid actual error output
}

// Test utility functions
static server_t *create_test_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    poll_manager_t *poll_manager = calloc(1, sizeof(poll_manager_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    parsed_info->frequence = 100;
    
    server->parsed_info = parsed_info;
    server->poll_manager = poll_manager;
    server->s_fd = 3;
    server->nfds = 0;
    server->current_tick = 0;
    
    return server;
}

static client_t *create_test_client(int fd)
{
    client_t *client = calloc(1, sizeof(client_t));
    struct pollfd *poll_fd = calloc(1, sizeof(struct pollfd));
    player_t *player = calloc(1, sizeof(player_t));
    
    client->client_fd = fd;
    client->client_poll = poll_fd;
    client->client_poll->fd = fd;
    client->client_poll->events = POLLIN;
    client->client_poll->revents = 0;
    client->player = player;
    client->type = AI;
    
    player->busy_until = 0;
    player->queue_size = 0;
    
    return client;
}

static void free_test_server(server_t *server)
{
    if (!server) return;
    
    if (server->poll_manager) {
        free(server->poll_manager->fds);
        free(server->poll_manager);
    }
    free(server->parsed_info);
    free(server);
}

static void free_test_client(client_t *client)
{
    if (!client) return;
    
    free(client->client_poll);
    free(client->player);
    free(client);
}

// =============================================================================
// FIND_CLIENT_BY_SOCKET TESTS (PUBLIC FUNCTION)
// =============================================================================

Test(find_client_by_socket, test_find_client_null_server)
{
    client_t *result = find_client_by_socket(NULL, 5);
    cr_assert_null(result);
}

Test(find_client_by_socket, test_find_client_null_client_list)
{
    server_t *server = create_test_server();
    server->client = NULL;
    
    client_t *result = find_client_by_socket(server, 5);
    cr_assert_null(result);
    
    free_test_server(server);
}

Test(find_client_by_socket, test_find_client_skip_server_socket)
{
    server_t *server = create_test_server();
    client_t *server_client = create_test_client(3); // Same as server->s_fd
    client_t *normal_client = create_test_client(5);
    
    server_client->next = normal_client;
    server->client = server_client;
    
    client_t *result = find_client_by_socket(server, 5);
    cr_assert_not_null(result);
    cr_assert_eq(result->client_fd, 5);
    
    free_test_client(normal_client);
    free_test_client(server_client);
    free_test_server(server);
}

Test(find_client_by_socket, test_find_client_found)
{
    server_t *server = create_test_server();
    client_t *client1 = create_test_client(4);
    client_t *client2 = create_test_client(5);
    client_t *client3 = create_test_client(6);
    
    client1->next = client2;
    client2->next = client3;
    server->client = client1;
    
    client_t *result = find_client_by_socket(server, 5);
    cr_assert_not_null(result);
    cr_assert_eq(result->client_fd, 5);
    
    free_test_client(client3);
    free_test_client(client2);
    free_test_client(client1);
    free_test_server(server);
}

Test(find_client_by_socket, test_find_client_not_found)
{
    server_t *server = create_test_server();
    client_t *client1 = create_test_client(4);
    client_t *client2 = create_test_client(5);
    
    client1->next = client2;
    server->client = client1;
    
    cr_redirect_stdout();
    client_t *result = find_client_by_socket(server, 99);
    cr_assert_null(result);
    
    free_test_client(client2);
    free_test_client(client1);
    free_test_server(server);
}

Test(find_client_by_socket, test_find_client_empty_list_after_server)
{
    server_t *server = create_test_server();
    client_t *server_client = create_test_client(3);
    
    server->client = server_client;
    server_client->next = NULL;
    
    cr_redirect_stdout();
    client_t *result = find_client_by_socket(server, 5);
    cr_assert_null(result);
    
    free_test_client(server_client);
    free_test_server(server);
}

// =============================================================================
// CHECK_CLIENT INTEGRATION TESTS (PUBLIC FUNCTION)
// =============================================================================

Test(check_client, test_check_client_basic_functionality)
{
    server_t *server = create_test_server();
    client_t *server_client = create_test_client(3);
    
    server->client = server_client;
    server->nfds = 0;
    
    reset_server_run_mocks();
    mock_poll_return = 0; // No activity
    mock_current_time.tv_sec = 1000;
    
    check_client(server);
    
    // Should call poll at least once
    cr_assert_eq(mock_poll_calls, 1);
    
    free_test_client(server_client);
    free_test_server(server);
}

Test(check_client, test_check_client_with_clients)
{
    server_t *server = create_test_server();
    client_t *server_client = create_test_client(3);
    client_t *normal_client = create_test_client(5);
    
    server_client->next = normal_client;
    server->client = server_client;
    server->nfds = 1;
    
    reset_server_run_mocks();
    mock_poll_return = 1;
    mock_current_time.tv_sec = 1000;
    
    // Simulate message from normal client
    normal_client->client_poll->revents = POLLIN;
    
    check_client(server);
    
    cr_assert_eq(mock_poll_calls, 1);
    cr_assert_eq(mock_get_message_calls, 1);
    
    free_test_client(normal_client);
    free_test_client(server_client);
    free_test_server(server);
}

Test(check_client, test_check_client_multiple_clients)
{
    server_t *server = create_test_server();
    client_t *server_client = create_test_client(3);
    client_t *client1 = create_test_client(4);
    client_t *client2 = create_test_client(5);
    client_t *client3 = create_test_client(6);
    
    server_client->next = client1;
    client1->next = client2;
    client2->next = client3;
    server->client = server_client;
    server->nfds = 3;
    
    reset_server_run_mocks();
    mock_poll_return = 2;
    mock_current_time.tv_sec = 1000;
    
    // Simulate messages from multiple clients
    client1->client_poll->revents = POLLIN;
    client3->client_poll->revents = POLLIN;
    
    check_client(server);
    
    cr_assert_eq(mock_poll_calls, 1);
    cr_assert_eq(mock_get_message_calls, 2);
    
    free_test_client(client3);
    free_test_client(client2);
    free_test_client(client1);
    free_test_client(server_client);
    free_test_server(server);
}

Test(check_client, test_check_client_no_activity)
{
    server_t *server = create_test_server();
    client_t *server_client = create_test_client(3);
    client_t *normal_client = create_test_client(5);
    
    server_client->next = normal_client;
    server->client = server_client;
    server->nfds = 1;
    
    reset_server_run_mocks();
    mock_poll_return = 0; // No activity
    mock_current_time.tv_sec = 1000;
    
    check_client(server);
    
    cr_assert_eq(mock_poll_calls, 1);
    cr_assert_eq(mock_get_message_calls, 0);
    
    free_test_client(normal_client);
    free_test_client(server_client);
    free_test_server(server);
}

Test(check_client, test_check_client_poll_manager_initialization)
{
    server_t *server = create_test_server();
    client_t *server_client = create_test_client(3);
    
    server->client = server_client;
    server->nfds = 2;
    server->poll_manager->fds = NULL;
    server->poll_manager->capacity = 0;
    server->poll_manager->needs_rebuild = true;
    
    reset_server_run_mocks();
    mock_poll_return = 0;
    mock_current_time.tv_sec = 1000;
    
    check_client(server);
    
    // Should initialize poll_manager
    cr_assert_not_null(server->poll_manager->fds);
    cr_assert_eq(server->poll_manager->capacity, 3); // nfds + 1
    
    free_test_client(server_client);
    free_test_server(server);
}