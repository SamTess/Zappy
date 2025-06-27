/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Server include
*/

#ifndef SRC_SERVER_INCLUDE_SERVER_H_
    #define SRC_SERVER_INCLUDE_SERVER_H_
    #include <sys/socket.h>
    #include <stdbool.h>
    #include "client.h"
    #include "parsing.h"
    #include "tile.h"
    #include "egg.h"

typedef struct zappy_client_s zappy_client_t;
typedef struct zappy_s zappy_t;

typedef struct poll_manager_s {
    struct pollfd *fds;
    int capacity;
    bool needs_rebuild;
} poll_manager_t;

typedef struct server_s {
    int nfds;
    int s_fd;
    struct sockaddr_in *serv_add;
    int should_run;
    poll_manager_t *poll_manager;
} server_t;

void update_game_tick(game_t *game, server_t *server, zappy_client_t *clients);
void finish_incantation(game_t *game, zappy_client_t *client,
    zappy_client_t *clients);
void display_help(void);
void remove_fd(server_t *server, zappy_client_t **clients, int fd);
void add_fd(zappy_client_t **clients, int fd);
void create_server(server_t *server, parsing_info_t *parsed_info,
    zappy_client_t **clients);
void server_err(char *msg);
void check_client(zappy_t *zappy);
void create_map(game_t *game, parsing_info_t *parsed_info);
void process_next_queued_command(game_t *game, server_t *server,
    zappy_client_t *client, zappy_client_t *clients);
void add_to_command_queue(zappy_client_t *client, char *command);
void free_node(zappy_client_t *node);
void init_server_eggs(game_t *game);
void free_all(server_t *server, game_t *game, zappy_client_t *clients,
    parsing_info_t *parsed_info);
int count_team(game_t *game);
void init_struct(zappy_client_t *new_c);
int add_string_to_write_buffer(circular_buffer_t *cb, char *msg);
void flush_client_write_buffer(client_t *client);
zappy_client_t *find_client_by_socket(zappy_client_t *clients,
    server_t *server, int socket_fd);
int init_zappy_client_struct(zappy_client_t *new_c);

#endif  // SRC_SERVER_INCLUDE_SERVER_H_
