/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_mocks - header file for mock function declarations
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
extern int mock_arr_len_result;

// Nouveaux mocks pour parse_command tests
extern bool __wrap_can_connect_return;
extern bool __wrap_send_pnw_command_to_all_called;
extern int __wrap_write_command_output_calls;
extern char *__wrap_write_command_output_last_msg;
extern bool __wrap_send_map_info_to_one_client_called;
extern bool __wrap_add_graphic_client_called;
extern bool __wrap_add_pending_cmd_called;
extern bool __wrap_add_to_command_queue_called;
extern bool __wrap_send_all_player_info_to_one_client_called;

// Mocks pour les commandes spécifiques
extern bool __wrap_command_msz_called;
extern bool __wrap_command_bct_called;
extern bool __wrap_command_mtc_called;
extern bool __wrap_command_tna_called;
extern bool __wrap_command_ppo_called;
extern bool __wrap_command_plv_called;
extern bool __wrap_command_pin_called;
extern bool __wrap_command_sgt_called;
extern bool __wrap_command_sst_called;
extern bool __wrap_start_incantation_called;
extern bool __wrap_command_pfk_called;
extern bool __wrap_broadcast_called;
extern bool __wrap_right_called;
extern bool __wrap_left_called;
extern bool __wrap_eject_called;
extern bool __wrap_connect_nbr_called;
extern bool __wrap_take_object_called;
extern bool __wrap_set_object_called;

// Mock functions
void reset_mocks(void);

#endif /* !TEST_MOCKS_H_ */
