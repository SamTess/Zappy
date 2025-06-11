/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command
*/

#ifndef COMMAND_H_
    #define COMMAND_H_
    #define MAX_LIFE_AFTER_FOOD 126
    #include "server.h"
    #include <stdbool.h>
    #include "client.h"

typedef struct command_data_s {
    const char **commands;
    void (**functions)(server_t *, client_t *, char *);
    int *times;
    enum client_type_e *accepted_types;
} command_data_t;

resource_type_t determine_type(char *resource_string);

void process_next_queued_command(server_t *server, client_t *client);
void cleanup_player_queue(player_t *player);
void cleanup_client(client_t *client);
char *tile_to_str(tile_t *tile);
void add_to_command_queue(client_t *client, char *command);
void get_message(server_t *server, client_t *user);
void execute_com(server_t *server, client_t *user, char *buffer);
void write_command_output(int client_fd, char *msg);
void forward(server_t *server, client_t *client, char *buffer);
void right(server_t *server, client_t *client, char *buffer);
void left(server_t *server, client_t *client, char *buffer);
void inventory(server_t *server, client_t *client, char *buffer);
void look(server_t *server, client_t *client, char *buffer);
void eject(server_t *server, client_t *client, char *buffer);
void connect_nbr(server_t *server, client_t *client, char *buffer);
void take_object(server_t *server, client_t *client, char *buffer);
void set_object(server_t *server, client_t *client, char *buffer);
void fork_c(server_t *server, client_t *client, char *buffer);
void broadcast(server_t *server, client_t *user, char *buffer);
int connect_nbr_srv(server_t *server, char *team);
client_t *find_client_by_id(server_t *server, int id);

// Death and starvation management functions
void handle_player_death(server_t *server, client_t *client);
bool check_player_starvation(server_t *server, client_t *client);
void start_incantation(server_t *server, client_t *client, char *buffer);
void finish_incantation(server_t *server, client_t *client);
bool can_start_incantation(server_t *server, client_t *client);
command_data_t get_command_data(void);
char *check_rota_tiles(client_t *user, server_t *server, int i, int j);

// client graphical commands
void command_msz(server_t *server, client_t *client, char *buffer);
void command_bct(server_t *server, client_t *client, char *buffer);
void command_mtc(server_t *server, client_t *client, char *buffer);
void command_tna(server_t *server, client_t *client, char *buffer);
void command_ppo(server_t *server, client_t *client, char *buffer);
void command_plv(server_t *server, client_t *client, char *buffer);
void command_pie(server_t *server, int x, int y, int result);
void command_pex(server_t *server, client_t *client);
void command_pin(server_t *server, client_t *client, char *buffer);

// send to all graphical clients
void send_mtc_to_all_graphical_clients(server_t *server);
void send_bct_to_all_graphical_clients(server_t *server, int x, int y);
void send_pnw_command_to_all(server_t *server, client_t *client);

#endif /* !COMMAND_H_ */
