/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_mocks - shared mock functions for unit tests
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/Server/include/egg.h"

// Mock tracking variables (extern declarations will be in test files)
int mock_write_calls = 0;
int mock_command_pex_calls = 0;
int mock_send_ppo_calls = 0;
int mock_send_edi_calls = 0;
int mock_tile_add_calls = 0;
int mock_tile_remove_calls = 0;
int mock_remove_egg_calls = 0;
int mock_command_pie_calls = 0;
int mock_command_seg_calls = 0;
int mock_send_plv_calls = 0;
int mock_can_start_calls = 0;
int mock_command_pic_calls = 0;
// Fork command mock variables
int mock_create_egg_calls = 0;
int mock_add_egg_calls = 0;
int mock_send_enw_calls = 0;
int mock_arr_len_calls = 0;
int mock_pgt_calls = 0;
int mock_pdr_calls = 0;
char *last_message = NULL;
char *last_seg_team = NULL;
int last_pie_x = -1;
int last_pie_y = -1;
int last_pie_result = -1;
int last_pic_x = -1;
int last_pic_y = -1;
int last_pic_level = -1;
bool mock_can_start_result = true;

// Shared mock functions
void __wrap_write_command_output(int client_fd, char *msg)
{
    (void)client_fd;
    mock_write_calls++;
    if (last_message) free(last_message);
    last_message = strdup(msg);
}

void __wrap_command_pex(void *server, void *client)
{
    (void)server;
    (void)client;
    mock_command_pex_calls++;
}

bool __wrap_send_ppo_command(void *server, int id)
{
    (void)server;
    (void)id;
    mock_send_ppo_calls++;
    return true;
}

void __wrap_send_edi_command(void *server, int id)
{
    (void)server;
    (void)id;
    mock_send_edi_calls++;
}

int __wrap_tile_add_player(void *tile, int player_id)
{
    (void)tile;
    (void)player_id;
    mock_tile_add_calls++;
    return 0;
}

int __wrap_tile_remove_player(void *tile, int player_id)
{
    (void)tile;
    (void)player_id;
    mock_tile_remove_calls++;
    return 0;
}

void __wrap_remove_egg(void *server, int id, void *tile)
{
    (void)server;
    (void)id;
    (void)tile;
    mock_remove_egg_calls++;
}

void __wrap_server_err(char *msg)
{
    (void)msg;
}

void __wrap_command_pie(void *server, int x, int y, int result)
{
    (void)server;
    mock_command_pie_calls++;
    last_pie_x = x;
    last_pie_y = y;
    last_pie_result = result;
}

void __wrap_command_seg(void *server, const char *team_name)
{
    (void)server;
    mock_command_seg_calls++;
    if (last_seg_team) free(last_seg_team);
    last_seg_team = strdup(team_name);
}

void __wrap_send_plv_to_all(void *server, void *client)
{
    (void)server;
    (void)client;
    mock_send_plv_calls++;
}

bool __wrap_can_start_incantation(void *server, void *client)
{
    (void)server;
    (void)client;
    mock_can_start_calls++;
    return mock_can_start_result;
}

void __wrap_command_pic(void *server, int x, int y, int level)
{
    (void)server;
    mock_command_pic_calls++;
    last_pic_x = x;
    last_pic_y = y;
    last_pic_level = level;
}

// Provide a generic __wrap_arr_len for tests that do not override it
__attribute__((weak)) int __wrap_arr_len(char **array) {
    int i = 0;
    if (!array) {
        printf("[DEBUG] __wrap_arr_len called with NULL array\n");
        return 0;
    }
    while (array[i])
        i++;
    printf("[DEBUG] __wrap_arr_len called, length = %d\n", i);
    return i;
}

// Note: Fork-specific mock functions (create_egg, add_egg, send_enw_command, arr_len)
// are implemented in test_fork.c with special behavior for testing

void __wrap_command_pgt(void *server, void *client, void *resource_type)
{
    (void)server;
    (void)client;
    (void)resource_type;
    mock_pgt_calls++;
}

void __wrap_command_pdr(void *server, void *client, void *resource_type)
{
    (void)server;
    (void)client;
    (void)resource_type;
    mock_pdr_calls++;
}
