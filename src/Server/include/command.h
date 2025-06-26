/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command
*/

#ifndef COMMAND_H_
    #define COMMAND_H_
    #define MAX_LIFE_AFTER_FOOD 126
    #include <stdbool.h>
    #include "game.h"
    #include "zappy.h"
    #include "graphical_commands.h"

typedef struct command_data_s {
    const char **commands;
    void (**functions)(game_t *, zappy_client_t *, zappy_client_t *, char **);
    int *times;
    enum client_type_e *accepted_types;
} command_data_t;

void forward(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void right(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void left(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer);
void inventory(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void look(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void eject(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void connect_nbr(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void take_object(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void set_object(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void fork_c(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void broadcast(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void start_incantation(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer);
void finish_incantation(game_t *game, zappy_client_t *client, zappy_client_t *clients);

int connect_nbr_srv(game_t *game, char *team);
bool can_connect(game_t *game, zappy_client_t *user, char *buffer, zappy_client_t *clients);
zappy_client_t *find_client_by_id(zappy_client_t *clients, int id);
void write_command_output_buffer(client_t *client, char *msg);
void handle_player_death(game_t *game, server_t *server, zappy_client_t *client, zappy_client_t *clients);
bool check_player_starvation(game_t *game, server_t *server, zappy_client_t *client, zappy_client_t *clients);
bool can_start_incantation(game_t *game, zappy_client_t *client, zappy_client_t *clients);
char *check_rota_tiles(player_t *player, game_t *game, int i, int j);
void add_pending_cmd(zappy_client_t *player, game_t *game,
    char *buffer, int cmd_index, zappy_client_t *clients);
void cleanup_pending(player_t *player);
char **str_to_word_arr(char *str, char *delim);
bool handle_socket_read(server_t *server, zappy_client_t **clients, zappy_client_t *user);
void write_command_output(int client_fd, char *msg);
resource_type_t determine_type(char *resource_string);
command_data_t get_command_data(void);
void process_next_queued_command(game_t *game, server_t *server, zappy_client_t *client, zappy_client_t *clients);
void cleanup_player_queue(player_t *player);
char *tile_to_str(tile_t *tile);
void get_message(server_t *server, zappy_client_t *user, zappy_client_t *clients, game_t *game);
void execute_com(game_t *game, zappy_client_t *user,
    zappy_client_t *clients, char *buffer);
#endif /* !COMMAND_H_ */
