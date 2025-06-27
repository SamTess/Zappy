/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** graphical_commands
*/

#ifndef GRAPHICAL_COMMANDS_H_
    #define GRAPHICAL_COMMANDS_H_

void command_sgt(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_msz(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_bct(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_mct(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_tna(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_ppo(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_plv(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_pin(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_sst(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer);
void command_pbc(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char *message);
void command_pex(game_t *game, zappy_client_t *client,
    zappy_client_t *clients);
void command_pdi(game_t *game, zappy_client_t *client,
    zappy_client_t *clients);
void command_pfk(game_t *game, zappy_client_t *client,
    zappy_client_t *clients);
void command_pdr(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, resource_type_t resource_type);
void command_pgt(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, resource_type_t resource_type);
void command_pic(game_t *game, zappy_client_t *clients,
    int *coords, int level);
void command_pie(game_t *game, zappy_client_t *clients,
    zappy_client_t *client, int result);
void command_seg(game_t *game, zappy_client_t *clients, char *team_name);

void send_msz_command(game_t *game, zappy_client_t *clients);
void send_enw_command_start(game_t *game, zappy_client_t *clients);
void send_mtc_to_all_graphical_clients(game_t *game, zappy_client_t *clients);
void send_bct_to_all_graphical_clients(game_t *game,
    zappy_client_t *clients, int x, int y);
void send_pnw_command_to_all(game_t *game, zappy_client_t *clients,
    zappy_client_t *client);
void send_tile_content_to_one_client(game_t *game, zappy_client_t *clients,
    zappy_client_t *client);
void send_all_player_info_to_one_client(game_t *game,
    zappy_client_t *clients, zappy_client_t *client);
bool send_ppo_command(game_t *game, zappy_client_t *clients, int id);
void send_plv_to_all(game_t *game, zappy_client_t *clients,
    zappy_client_t *client);
void send_pin_to_all(game_t *game, zappy_client_t *clients,
    zappy_client_t *client);
void send_edi_command(game_t *game, zappy_client_t *clients, int egg_id);
void send_smg_command(game_t *game, zappy_client_t *clients, char *msg);
void send_tna_command(game_t *game, zappy_client_t *clients,
    zappy_client_t *client);
void send_pnw_command(game_t *game, zappy_client_t *clients,
    zappy_client_t *client, zappy_client_t *recipient);
void send_pin_command(game_t *game, zappy_client_t *clients,
    zappy_client_t *client, zappy_client_t *recipient);
void send_plv_command(game_t *game, zappy_client_t *clients,
    zappy_client_t *client, zappy_client_t *recipient);
void send_ebo_command(game_t *game, zappy_client_t *clients, int egg_id);
void send_enw_command(game_t *game, zappy_client_t *clients,
    zappy_client_t *parent, int egg_id);


#endif /* !GRAPHICAL_COMMANDS_H_ */
