/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_mocks.h - Shared mock function declarations for unit tests
*/

#ifndef TEST_MOCKS_H_
#define TEST_MOCKS_H_

#include <stdbool.h>

// Mock tracking variables
extern int mock_write_calls;
extern int mock_command_pex_calls;
extern int mock_send_ppo_calls;
extern int mock_send_edi_calls;
extern int mock_tile_add_calls;
extern int mock_tile_remove_calls;
extern int mock_remove_egg_calls;
extern int mock_command_pie_calls;
extern int mock_command_seg_calls;
extern int mock_send_plv_calls;
extern int mock_can_start_calls;
extern int mock_command_pic_calls;
extern int mock_create_egg_calls;
extern int mock_add_egg_calls;
extern int mock_send_enw_calls;
extern int mock_arr_len_calls;
extern int mock_pgt_calls;
extern int mock_pdr_calls;

// Mock result variables
extern char *last_message;
extern char *last_seg_team;
extern int last_pie_x;
extern int last_pie_y;
extern int last_pie_result;
extern int last_pic_x;
extern int last_pic_y;
extern int last_pic_level;
extern bool mock_can_start_result;

// Mock function declarations - shared functions 
void __wrap_write_command_output(int client_fd, char *msg);
void __wrap_command_pex(void *server, void *client);
bool __wrap_send_ppo_command(void *server, int id);
void __wrap_send_ppo_command_void(void *server, void *client); // Alternative signature
void __wrap_send_edi_command(void *server, int id);
int __wrap_tile_add_player(void *tile, int player_id);
int __wrap_tile_remove_player(void *tile, int player_id);
void __wrap_remove_egg(void *server, int id, void *tile);
void __wrap_server_err(char *msg);
void __wrap_command_pie(void *server, int x, int y, int result);
void __wrap_command_seg(void *server, const char *team_name);
void __wrap_send_plv_to_all(void *server, void *client);
bool __wrap_can_start_incantation(void *server, void *client);
void __wrap_command_pic(void *server, int x, int y, int level);
void __wrap_command_pgt(void *server, void *client, void *resource_type);
void __wrap_command_pdr(void *server, void *client, void *resource_type);

// Note: arr_len, create_egg, add_egg, and send_enw_command are implemented
// separately in each test file when needed

#endif /* !TEST_MOCKS_H_ */
