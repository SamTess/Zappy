/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Server include
*/

#ifndef SERVER_H_
    #define SERVER_H_
    #include <sys/socket.h>
    #include "client.h"
    #include "parsing.h"
    #include "tile.h"
    #include "egg.h"

typedef struct server_s {
    int nfds;
    int s_fd;
    struct sockaddr_in *serv_add;
    client_t *client;
    egg_t *eggs;
    tile_t **map;
    int current_tick;
    parsing_info_t *parsed_info;
    int *total_resources;
    int *current_resources;
} server_t;

void update_game_tick(server_t *server);
void finish_incantation(server_t *server, client_t *client);
void display_help(void);
void remove_fd(server_t *server, int fd);
void add_fd(server_t *server, int fd);
void create_server(server_t *server, parsing_info_t *parsed_info);
void server_err(char *msg);
void print_co(char *client_ip, struct sockaddr_in *client_addr,
    client_t *new_client);
void check_client(server_t *server);
void create_map(server_t *server, parsing_info_t *parsed_info);
void init_new_player_pos(server_t *server, client_t *new_client);
void process_next_queued_command(server_t *server, client_t *client);
void add_to_command_queue(client_t *client, char *command);
void free_node(client_t *node);
void init_server_eggs(server_t *n_server);

#endif /* !SERVER_H_ */
