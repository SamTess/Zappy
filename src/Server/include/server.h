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

typedef struct server_s {
    int nfds;
    int s_fd;
    struct sockaddr_in *serv_add;
    client_t *client;
    tile_t **map;
} server_t;

void display_help(void);
void remove_fd(server_t *server, int fd);
void add_fd(server_t *server, int fd);
void create_server(server_t *server, parsing_info_t *parsed_info);
void malloc_failed(int i);
void server_err(char *msg);
void print_co(char *client_ip, struct sockaddr_in *client_addr,
    client_t *new_client);
void check_client(server_t *server);
void create_map(server_t *server, parsing_info_t *parsed_info);

#endif /* !SERVER_H_ */
