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
void send_bct_command(server_t *server, client_t *client, int x, int y);
void send_sgt_command(server_t *server, client_t *client);
void send_tna_command(server_t *server, client_t *client);
void send_pnw_command(server_t *server, client_t *client, client_t *recipient);
void send_pin_command(server_t *server, client_t *client, client_t *recipient);
void send_plv_command(server_t *server, client_t *client, client_t *recipient);
void send_ebo_command(server_t *server, int egg_id);

void send_tile_content_to_one_client(server_t *server, client_t *client);
void send_team_names_to_one_client(server_t *server, client_t *client);
void send_all_player_info_to_one_client(server_t *server, client_t *client);

#endif /* !GRAPHICAL_COMMANDS_H_ */
