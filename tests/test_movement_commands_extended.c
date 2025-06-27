/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Extended movement commands tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "test_mocks.h"
#include "../src/Server/include/command.h"
#include "../src/Server/include/parsing.h"
#include "../src/Server/include/graphical_commands.h"

// Test Suite for Forward Command
Test(forward_command, valid_forward_up)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 5);
    cr_assert_eq(client.player->pos_y, 4);
    cr_assert_eq(client.player->rotation, UP);
}

Test(forward_command, valid_forward_down)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, DOWN, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 5);
    cr_assert_eq(client.player->pos_y, 6);
    cr_assert_eq(client.player->rotation, DOWN);
}

Test(forward_command, valid_forward_left)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, LEFT, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 4);
    cr_assert_eq(client.player->pos_y, 5);
    cr_assert_eq(client.player->rotation, LEFT);
}

Test(forward_command, valid_forward_right)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, RIGHT, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 6);
    cr_assert_eq(client.player->pos_y, 5);
    cr_assert_eq(client.player->rotation, RIGHT);
}

Test(forward_command, wrap_position_top_edge)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 0, UP, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 5);
    cr_assert_eq(client.player->pos_y, 9); // Wrapped to bottom
}

Test(forward_command, wrap_position_bottom_edge)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 9, DOWN, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 5);
    cr_assert_eq(client.player->pos_y, 0); // Wrapped to top
}

Test(forward_command, wrap_position_left_edge)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(0, 5, LEFT, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 9); // Wrapped to right
    cr_assert_eq(client.player->pos_y, 5);
}

Test(forward_command, wrap_position_right_edge)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(9, 5, RIGHT, 1);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_eq(client.player->pos_x, 0); // Wrapped to left
    cr_assert_eq(client.player->pos_y, 5);
}

Test(forward_command, null_client)
{
    server_t server = create_mock_server(10, 10);
    char *buffer[] = {"forward", NULL};
    
    forward(&server, NULL, buffer);
    
    // Should return without crashing
    cr_assert(1); // Test passes if no crash
}

Test(forward_command, null_player, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_without_player();
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    // Should send "ko\n" to client
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(forward_command, invalid_arguments, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"forward", "extra", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(forward_command, invalid_rotation, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, 99, 1); // Invalid rotation
    char *buffer[] = {"forward", NULL};
    
    forward(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

// Test Suite for Left Command
Test(left_command, valid_left_from_up)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"left", NULL};
    
    left(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, LEFT);
    cr_assert_eq(client.player->pos_x, 5); // Position unchanged
    cr_assert_eq(client.player->pos_y, 5);
}

Test(left_command, valid_left_from_down)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, DOWN, 1);
    char *buffer[] = {"left", NULL};
    
    left(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, RIGHT);
}

Test(left_command, valid_left_from_left)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, LEFT, 1);
    char *buffer[] = {"left", NULL};
    
    left(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, DOWN);
}

Test(left_command, valid_left_from_right)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, RIGHT, 1);
    char *buffer[] = {"left", NULL};
    
    left(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, UP);
}

Test(left_command, null_client)
{
    server_t server = create_mock_server(10, 10);
    char *buffer[] = {"left", NULL};
    
    left(&server, NULL, buffer);
    
    cr_assert(1); // Test passes if no crash
}

Test(left_command, null_server, .init = cr_redirect_stderr)
{
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"left", NULL};
    
    left(NULL, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(left_command, null_player, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_without_player();
    char *buffer[] = {"left", NULL};
    
    left(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(left_command, invalid_arguments, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"left", "extra", NULL};
    
    left(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(left_command, invalid_rotation, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, 99, 1);
    char *buffer[] = {"left", NULL};
    
    left(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

// Test Suite for Right Command
Test(right_command, valid_right_from_up)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"right", NULL};
    
    right(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, RIGHT);
    cr_assert_eq(client.player->pos_x, 5); // Position unchanged
    cr_assert_eq(client.player->pos_y, 5);
}

Test(right_command, valid_right_from_down)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, DOWN, 1);
    char *buffer[] = {"right", NULL};
    
    right(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, LEFT);
}

Test(right_command, valid_right_from_left)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, LEFT, 1);
    char *buffer[] = {"right", NULL};
    
    right(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, UP);
}

Test(right_command, valid_right_from_right)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, RIGHT, 1);
    char *buffer[] = {"right", NULL};
    
    right(&server, &client, buffer);
    
    cr_assert_eq(client.player->rotation, DOWN);
}

Test(right_command, null_client)
{
    server_t server = create_mock_server(10, 10);
    char *buffer[] = {"right", NULL};
    
    right(&server, NULL, buffer);
    
    cr_assert(1); // Test passes if no crash
}

Test(right_command, null_server, .init = cr_redirect_stderr)
{
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"right", NULL};
    
    right(NULL, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(right_command, null_player, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_without_player();
    char *buffer[] = {"right", NULL};
    
    right(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(right_command, invalid_arguments, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *buffer[] = {"right", "extra", NULL};
    
    right(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

Test(right_command, invalid_rotation, .init = cr_redirect_stderr)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, 99, 1);
    char *buffer[] = {"right", NULL};
    
    right(&server, &client, buffer);
    
    cr_assert_str_eq(get_last_written_output(), "ko\n");
}

// Integration Tests
Test(movement_integration, full_rotation_cycle)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *left_cmd[] = {"left", NULL};
    
    // Start facing UP, turn left 4 times to complete full rotation
    left(&server, &client, left_cmd);
    cr_assert_eq(client.player->rotation, LEFT);
    
    left(&server, &client, left_cmd);
    cr_assert_eq(client.player->rotation, DOWN);
    
    left(&server, &client, left_cmd);
    cr_assert_eq(client.player->rotation, RIGHT);
    
    left(&server, &client, left_cmd);
    cr_assert_eq(client.player->rotation, UP);
}

Test(movement_integration, move_and_rotate_sequence)
{
    server_t server = create_mock_server(10, 10);
    client_t client = create_mock_client_with_player(5, 5, UP, 1);
    char *forward_cmd[] = {"forward", NULL};
    char *right_cmd[] = {"right", NULL};
    
    // Move forward (up), turn right, move forward (right)
    forward(&server, &client, forward_cmd);
    cr_assert_eq(client.player->pos_x, 5);
    cr_assert_eq(client.player->pos_y, 4);
    
    right(&server, &client, right_cmd);
    cr_assert_eq(client.player->rotation, RIGHT);
    
    forward(&server, &client, forward_cmd);
    cr_assert_eq(client.player->pos_x, 6);
    cr_assert_eq(client.player->pos_y, 4);
}

Test(movement_integration, corner_wrapping_test)
{
    server_t server = create_mock_server(3, 3);
    client_t client = create_mock_client_with_player(0, 0, UP, 1);
    char *forward_cmd[] = {"forward", NULL};
    char *left_cmd[] = {"left", NULL};
    
    // At top-left corner, move up (should wrap to bottom)
    forward(&server, &client, forward_cmd);
    cr_assert_eq(client.player->pos_x, 0);
    cr_assert_eq(client.player->pos_y, 2);
    
    // Turn left and move (should wrap to right edge)
    left(&server, &client, left_cmd);
    forward(&server, &client, forward_cmd);
    cr_assert_eq(client.player->pos_x, 2);
    cr_assert_eq(client.player->pos_y, 2);
}