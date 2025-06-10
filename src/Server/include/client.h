/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** client
*/

#ifndef CLIENT_H_
    #define CLIENT_H_
    #include <sys/poll.h>
    #include <netinet/in.h>
    #include "player.h"

enum client_type_e {
    GRAPHICAL,
    AI
};

typedef struct graphical_client_s {
    client_t *client;
    struct graphical_client_s *next;
} graphical_client_t;

typedef struct client_s {
    int client_id;
    int client_fd;
    struct pollfd *client_poll;
    struct sockaddr_in *client_add;
    char *team;
    player_t *player;
    struct client_s *next;
    bool is_fully_connected;
    enum client_type_e type;
} client_t;

void send_message_to_all_graphic(server_t *server, char *message);
void remove_graphic_client(server_t *server, client_t *client);
void add_graphic_client(server_t *server, client_t *client);
void send_map_info_to_one_client(server_t *server, client_t *client);

#endif /* !CLIENT_H_ */
