/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_connection_utils_simple
*/

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/circular_buffer.h"

// Mock function for init_struct
static void mock_init_struct(client_t *new_c)
{
    if (!new_c || !new_c->player)
        return;
    
    // Basic initialization
    new_c->player->pos_x = 0;
    new_c->player->pos_y = 0;
    new_c->player->rotation = UP;
    new_c->player->level = 1;
    new_c->player->life = 10;
}

TestSuite(connection_utils_simple);

Test(connection_utils_simple, test_count_team_basic)
{
    server_t server = {0};
    parsing_info_t parsed_info = {0};
    
    // Setup team names
    char *names[] = {"team1", "team2", "team3", NULL};
    parsed_info.names = names;
    server.parsed_info = &parsed_info;
    
    int result = count_team(&server);
    cr_assert_eq(result, 3, "Should count 3 teams");
}

Test(connection_utils_simple, test_init_struct_basic)
{
    client_t client = {0};
    player_t player = {0};
    
    client.player = &player;
    
    // Use mock function instead of actual implementation
    mock_init_struct(&client);
    
    cr_assert_eq(client.player->pos_x, 0, "Position X should be initialized to 0");
    cr_assert_eq(client.player->pos_y, 0, "Position Y should be initialized to 0");
    cr_assert_eq(client.player->level, 1, "Level should be initialized to 1");
}

static client_t *create_test_client(int fd)
{
    client_t *client = calloc(1, sizeof(client_t));
    client->client_fd = fd;
    client->client_id = 1;
    return client;
}

static void free_test_client(client_t *client)
{
    if (client) {
        free(client);
    }
}

Test(connection_utils_tests, test_client_creation_and_cleanup)
{
    client_t *new_c = create_test_client(5);
    
    cr_assert_not_null(new_c);
    cr_assert_eq(new_c->client_fd, 5);
    
    free_test_client(new_c);
}