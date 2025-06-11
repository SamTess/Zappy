/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** graphical_commands
*/

#ifndef GRAPHICAL_COMMANDS_H_
    #define GRAPHICAL_COMMANDS_H_
    #include "server.h"
    #include "client.h"

void send_msz_command(server_t *server, client_t *client);
void command_sgt(server_t *server, client_t *client, char *buffer);
void send_tna_command(server_t *server, client_t *client);
void send_pnw_command(server_t *server, client_t *client, client_t *recipient);
void send_pin_command(server_t *server, client_t *client, client_t *recipient);
void send_plv_command(server_t *server, client_t *client, client_t *recipient);
void send_ebo_command(server_t *server, int egg_id);
void command_pic(server_t *server, int x, int y, int level);
void command_pbc(server_t *server, client_t *client, char *buffer);
void command_seg(server_t *server, const char *team_name);


void send_tile_content_to_one_client(server_t *server, client_t *client);
void send_team_names_to_one_client(server_t *server, client_t *client);
void send_all_player_info_to_one_client(server_t *server, client_t *client);
bool send_ppo_command(server_t *server, int id);
void send_plv_to_all(server_t *server, client_t *client);
void send_pin_to_all(server_t *server, client_t *client);
void send_edi_command(server_t *server, int egg_id);
void send_smg_command(server_t *server, const char *msg);

#endif /* !GRAPHICAL_COMMANDS_H_ */
