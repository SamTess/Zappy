/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_graphical_commands - Tests for graphical commands with 0% coverage
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/tile.h"
#include "test_mocks.h"

// External mock variables
extern int mock_write_calls;
extern char *last_message;

static void reset_graphical_mocks(void)
{
    mock_write_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

// Helper functions
static server_t *create_graphical_server(void)
{
    server_t *server = calloc(1, sizeof(server_t));
    parsing_info_t *parsed_info = calloc(1, sizeof(parsing_info_t));
    
    parsed_info->width = 10;
    parsed_info->height = 10;
    server->parsed_info = parsed_info;
    
    // Allocate basic map
    server->map = calloc(10, sizeof(tile_t *));
    for (int y = 0; y < 10; y++) {
        server->map[y] = calloc(10, sizeof(tile_t));
    }
    
    return server;
}

static client_t *create_graphical_client(void)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->client_id = 1;
    client->client_fd = 5;
    client->type = GRAPHICAL;
    
    return client;
}

static void free_graphical_server(server_t *server)
{
    if (!server) return;
    
    if (server->map) {
        for (int y = 0; y < 10; y++) {
            free(server->map[y]);
        }
        free(server->map);
    }
    
    free(server->parsed_info);
    free(server);
}

// =============================================================================
// COMMAND_MSZ TESTS (0% coverage)
// =============================================================================

Test(command_msz, test_msz_null_server)
{
    client_t *client = create_graphical_client();
    char *buffer[] = {"msz", NULL};
    
    reset_graphical_mocks();
    
    command_msz(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
}

Test(command_msz, test_msz_success)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"msz", NULL};
    
    reset_graphical_mocks();
    
    command_msz(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    cr_assert(strstr(last_message, "msz") != NULL);
    
    free_graphical_server(server);
    free(client);
}

// =============================================================================
// COMMAND_BCT TESTS (0% coverage)
// =============================================================================

Test(command_bct, test_bct_null_server)
{
    client_t *client = create_graphical_client();
    char *buffer[] = {"bct", "5", "5", NULL};
    
    reset_graphical_mocks();
    
    command_bct(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
}

Test(command_bct, test_bct_invalid_coords)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"bct", "invalid", "coords", NULL};
    
    reset_graphical_mocks();
    
    command_bct(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_graphical_server(server);
    free(client);
}

Test(command_bct, test_bct_success)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"bct", "5", "5", NULL};
    
    reset_graphical_mocks();
    
    command_bct(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    
    free_graphical_server(server);
    free(client);
}

// =============================================================================
// COMMAND_TNA TESTS (0% coverage)
// =============================================================================

Test(command_tna, test_tna_null_server)
{
    client_t *client = create_graphical_client();
    char *buffer[] = {"tna", NULL};
    
    reset_graphical_mocks();
    
    command_tna(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
}

Test(command_tna, test_tna_success)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"tna", NULL};
    
    reset_graphical_mocks();
    
    command_tna(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    
    free_graphical_server(server);
    free(client);
}

// =============================================================================
// COMMAND_SST TESTS (0% coverage)
// =============================================================================

Test(command_sst, test_sst_null_server)
{
    client_t *client = create_graphical_client();
    char *buffer[] = {"sst", "50", NULL};
    
    reset_graphical_mocks();
    
    command_sst(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
}

Test(command_sst, test_sst_invalid_time)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"sst", "invalid", NULL};
    
    reset_graphical_mocks();
    
    command_sst(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_graphical_server(server);
    free(client);
}

Test(command_sst, test_sst_success)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"sst", "50", NULL};
    
    reset_graphical_mocks();
    
    command_sst(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_not_null(last_message);
    
    free_graphical_server(server);
    free(client);
}

// =============================================================================
// COMMAND_PIN TESTS (0% coverage)
// =============================================================================

Test(command_pin, test_pin_null_server)
{
    client_t *client = create_graphical_client();
    char *buffer[] = {"pin", "1", NULL};
    
    reset_graphical_mocks();
    
    command_pin(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
}

Test(command_pin, test_pin_player_not_found)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"pin", "999", NULL};
    
    reset_graphical_mocks();
    
    command_pin(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_graphical_server(server);
    free(client);
}

// =============================================================================
// COMMAND_PPO TESTS (0% coverage)
// =============================================================================

Test(command_ppo, test_ppo_null_server)
{
    client_t *client = create_graphical_client();
    char *buffer[] = {"ppo", "1", NULL};
    
    reset_graphical_mocks();
    
    command_ppo(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
}

Test(command_ppo, test_ppo_player_not_found)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"ppo", "999", NULL};
    
    reset_graphical_mocks();
    
    command_ppo(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_graphical_server(server);
    free(client);
}

// =============================================================================
// COMMAND_PLV TESTS (0% coverage)
// =============================================================================

Test(command_plv, test_plv_null_server)
{
    client_t *client = create_graphical_client();
    char *buffer[] = {"plv", "1", NULL};
    
    reset_graphical_mocks();
    
    command_plv(NULL, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free(client);
}

Test(command_plv, test_plv_player_not_found)
{
    server_t *server = create_graphical_server();
    client_t *client = create_graphical_client();
    char *buffer[] = {"plv", "999", NULL};
    
    reset_graphical_mocks();
    
    command_plv(server, client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    
    free_graphical_server(server);
    free(client);
}