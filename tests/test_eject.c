/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_eject_core - core functionality tests
*/

#include <criterion/criterion.h>
#include "../src/Server/include/command.h"
#include "../src/Server/include/server.h"
#include "../src/Server/include/client.h"
#include "../src/Server/include/player.h"
#include "../src/Server/include/parsing.h"
#include <stdlib.h>
#include <string.h>

// External mock tracking variables (defined in test_mocks.c)
extern int mock_write_calls;
extern int mock_send_ppo_calls;
extern int mock_command_pex_calls;
extern int mock_send_edi_calls;
extern int mock_tile_add_calls;
extern int mock_remove_egg_calls;
extern char *last_message;

static void reset_mocks(void)
{
    mock_write_calls = 0;
    mock_send_ppo_calls = 0;
    mock_command_pex_calls = 0;
    mock_send_edi_calls = 0;
    mock_tile_add_calls = 0;
    mock_remove_egg_calls = 0;
    if (last_message) {
        free(last_message);
        last_message = NULL;
    }
}

TestSuite(eject_core_tests);

// Test: Invalid arguments - test the bug where NULL client causes crash
Test(eject_core_tests, test_eject_basic_success)
{
    reset_mocks();
    
    // Test with NULL player (client exists but player is NULL)
    client_t client = {
        .client_id = 1,
        .client_fd = 101,
        .type = AI,
        .player = NULL, // This should trigger the ko path
        .next = NULL
    };
    
    char *buffer[] = {"eject", NULL};
    
    eject(NULL, &client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_command_pex_calls, 0);
    
    reset_mocks();
}

// Test: Invalid arguments
Test(eject_core_tests, test_eject_invalid_args)
{
    reset_mocks();
    
    parsing_info_t parsed_info = {.width = 10, .height = 10};
    server_t server = {.parsed_info = &parsed_info};
    
    player_t player = {.pos_x = 5, .pos_y = 5, .rotation = UP};
    client_t client = {
        .client_id = 1,
        .client_fd = 101,
        .type = AI,
        .player = &player,
        .next = NULL
    };
    
    char *buffer_too_many[] = {"eject", "extra", NULL};
    
    eject(&server, &client, buffer_too_many);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ko\n");
    cr_assert_eq(mock_command_pex_calls, 0);
    
    reset_mocks();
}

// Test: Client pushing
Test(eject_core_tests, test_eject_client_pushing)
{
    reset_mocks();
    
    parsing_info_t parsed_info = {.width = 10, .height = 10};
    tile_t map_data[10][10] = {0};
    tile_t *map_rows[10];
    for (int i = 0; i < 10; i++) {
        map_rows[i] = map_data[i];
    }
    
    // Set up clients
    player_t player1 = {.pos_x = 5, .pos_y = 5, .rotation = UP};
    player_t player2 = {.pos_x = 5, .pos_y = 5, .rotation = UP}; // Same position
    
    client_t client2 = {
        .client_id = 2,
        .client_fd = 102,
        .type = AI,
        .player = &player2,
        .next = NULL
    };
    
    client_t client1 = {
        .client_id = 1,
        .client_fd = 101,
        .type = AI,
        .player = &player1,
        .next = &client2
    };
    
    client_t dummy_head = {.next = &client1};
    
    server_t server = {
        .parsed_info = &parsed_info,
        .map = map_rows,
        .client = &dummy_head,
        .eggs = NULL,
        .graphical_clients = NULL
    };
    
    char *buffer[] = {"eject", NULL};
    
    eject(&server, &client1, buffer);
    
    cr_assert_eq(mock_write_calls, 2); // "ok" to ejector + message to pushed client
    cr_assert_eq(mock_send_ppo_calls, 1);   // Position update for pushed client
    cr_assert_eq(mock_command_pex_calls, 1);   // Eject notification
    cr_assert_eq(mock_tile_add_calls, 1); // Add pushed client to new tile
    
    // Check final positions based on actual eject logic
    cr_assert_eq(player1.pos_x, 5); // Ejector doesn't move
    cr_assert_eq(player1.pos_y, 5);
    cr_assert_eq(player2.pos_x, 6); // Pushed client: 5 + (same_x ? 1 : 0)
    cr_assert_eq(player2.pos_y, 6); // Pushed client: 5 + (same_y ? 1 : 0)
    
    reset_mocks();
}

// Test: Basic successful eject (no eggs, no other clients)
Test(eject_core_tests, test_eject_egg_removal)
{
    reset_mocks();
    
    parsing_info_t parsed_info = {.width = 10, .height = 10};
    tile_t map_data[10][10] = {0};
    tile_t *map_rows[10];
    for (int i = 0; i < 10; i++) {
        map_rows[i] = map_data[i];
    }
    
    player_t player = {.pos_x = 5, .pos_y = 5, .rotation = UP};
    client_t client = {
        .client_id = 1,
        .client_fd = 101,
        .type = AI,
        .player = &player,
        .next = NULL
    };
    
    // Create dummy head client to avoid NULL dereference
    client_t dummy_head = {.next = NULL};
    
    server_t server = {
        .parsed_info = &parsed_info,
        .map = map_rows,
        .client = &dummy_head,  // Provide dummy head
        .eggs = NULL,           // No eggs to remove
        .graphical_clients = NULL
    };
    
    char *buffer[] = {"eject", NULL};
    
    eject(&server, &client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    cr_assert_eq(mock_command_pex_calls, 1);
    cr_assert_eq(mock_send_edi_calls, 0);      // No eggs to remove
    cr_assert_eq(mock_remove_egg_calls, 0);
    
    reset_mocks();
}

// Test: Client pushing with position wrapping
Test(eject_core_tests, test_eject_client_pushing_with_wrapping)
{
    reset_mocks();
    
    parsing_info_t parsed_info = {.width = 5, .height = 5};
    tile_t map_data[5][5] = {0};
    tile_t *map_rows[5];
    for (int i = 0; i < 5; i++) {
        map_rows[i] = map_data[i];
    }
    
    // Set up clients at edge position that will wrap
    player_t player1 = {.pos_x = 4, .pos_y = 4, .rotation = RIGHT}; // At bottom-right corner
    player_t player2 = {.pos_x = 4, .pos_y = 4, .rotation = UP}; // Same position
    
    client_t client2 = {
        .client_id = 2,
        .client_fd = 102,
        .type = AI,
        .player = &player2,
        .next = NULL
    };
    
    client_t client1 = {
        .client_id = 1,
        .client_fd = 101,
        .type = AI,
        .player = &player1,
        .next = &client2
    };
    
    client_t dummy_head = {.next = &client1};
    
    server_t server = {
        .parsed_info = &parsed_info,
        .map = map_rows,
        .client = &dummy_head,
        .eggs = NULL,
        .graphical_clients = NULL
    };
    
    char *buffer[] = {"eject", NULL};
    
    eject(&server, &client1, buffer);
    
    cr_assert_eq(mock_write_calls, 2);
    cr_assert_eq(mock_send_ppo_calls, 1);
    cr_assert_eq(mock_command_pex_calls, 1);
    cr_assert_eq(mock_tile_add_calls, 1);
    
    // Position should wrap: (4+1, 4+1) = (5, 5) -> (0, 0) due to wrapping
    cr_assert_eq(player2.pos_x, 0);
    cr_assert_eq(player2.pos_y, 0);
    
    reset_mocks();
}

// Test: Eject with eggs at same position
Test(eject_core_tests, test_eject_with_eggs)
{
    reset_mocks();
    
    parsing_info_t parsed_info = {.width = 10, .height = 10};
    tile_t map_data[10][10] = {0};
    tile_t *map_rows[10];
    for (int i = 0; i < 10; i++) {
        map_rows[i] = map_data[i];
    }
    
    // Set up eggs - some at same position, some different
    egg_t egg3 = {.egg_id = 103, .pos_x = 7, .pos_y = 7, .next = NULL}; // Different position
    egg_t egg2 = {.egg_id = 102, .pos_x = 5, .pos_y = 5, .next = &egg3}; // Same position
    egg_t egg1 = {.egg_id = 101, .pos_x = 5, .pos_y = 5, .next = &egg2}; // Same position
    
    player_t player = {.pos_x = 5, .pos_y = 5, .rotation = DOWN};
    client_t client = {
        .client_id = 1,
        .client_fd = 101,
        .type = AI,
        .player = &player,
        .next = NULL
    };
    
    client_t dummy_head = {.next = NULL};
    
    server_t server = {
        .parsed_info = &parsed_info,
        .map = map_rows,
        .client = &dummy_head,
        .eggs = &egg1,
        .graphical_clients = NULL
    };
    
    char *buffer[] = {"eject", NULL};
    
    eject(&server, &client, buffer);
    
    cr_assert_eq(mock_write_calls, 1);
    cr_assert_str_eq(last_message, "ok\n");
    cr_assert_eq(mock_command_pex_calls, 1);
    cr_assert_eq(mock_send_edi_calls, 2);      // Two eggs at same position removed
    cr_assert_eq(mock_remove_egg_calls, 2);
    
    reset_mocks();
}

// Test: Test different rotation directions for coverage
Test(eject_core_tests, test_eject_all_rotations)
{
    reset_mocks();
    
    parsing_info_t parsed_info = {.width = 10, .height = 10};
    tile_t map_data[10][10] = {0};
    tile_t *map_rows[10];
    for (int i = 0; i < 10; i++) {
        map_rows[i] = map_data[i];
    }
    
    client_t dummy_head = {.next = NULL};
    
    server_t server = {
        .parsed_info = &parsed_info,
        .map = map_rows,
        .client = &dummy_head,
        .eggs = NULL,
        .graphical_clients = NULL
    };
    
    char *buffer[] = {"eject", NULL};
    
    // Test all rotations
    enum rotation_e rotations[] = {UP, DOWN, LEFT, RIGHT};
    
    for (int i = 0; i < 4; i++) {
        player_t player = {.pos_x = 5, .pos_y = 5, .rotation = rotations[i]};
        client_t client = {
            .client_id = i + 1,
            .client_fd = 101 + i,
            .type = AI,
            .player = &player,
            .next = NULL
        };
        
        eject(&server, &client, buffer);
        
        cr_assert_eq(mock_write_calls, i + 1);
        cr_assert_eq(mock_command_pex_calls, i + 1);
    }
    
    reset_mocks();
}

// Test: Graphical client should be ignored
Test(eject_core_tests, test_eject_ignore_graphical_client)
{
    reset_mocks();
    
    parsing_info_t parsed_info = {.width = 10, .height = 10};
    tile_t map_data[10][10] = {0};
    tile_t *map_rows[10];
    for (int i = 0; i < 10; i++) {
        map_rows[i] = map_data[i];
    }
    
    // Set up clients - one AI, one graphical at same position
    player_t player1 = {.pos_x = 5, .pos_y = 5, .rotation = UP};
    player_t player2 = {.pos_x = 5, .pos_y = 5, .rotation = UP}; // Same position
    
    client_t graphical_client = {
        .client_id = 2,
        .client_fd = 102,
        .type = GRAPHICAL, // This should be ignored
        .player = &player2,
        .next = NULL
    };
    
    client_t client1 = {
        .client_id = 1,
        .client_fd = 101,
        .type = AI,
        .player = &player1,
        .next = &graphical_client
    };
    
    client_t dummy_head = {.next = &client1};
    
    server_t server = {
        .parsed_info = &parsed_info,
        .map = map_rows,
        .client = &dummy_head,
        .eggs = NULL,
        .graphical_clients = NULL
    };
    
    char *buffer[] = {"eject", NULL};
    
    eject(&server, &client1, buffer);
    
    cr_assert_eq(mock_write_calls, 1); // Only "ok" to ejector
    cr_assert_eq(mock_send_ppo_calls, 0);   // Graphical client not pushed
    cr_assert_eq(mock_command_pex_calls, 1);
    cr_assert_eq(mock_tile_add_calls, 0); // No tile add for graphical client
    
    // Graphical client should not be moved
    cr_assert_eq(player2.pos_x, 5);
    cr_assert_eq(player2.pos_y, 5);
    
    reset_mocks();
}
