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

typedef struct client_s {
    int client_id;
    int client_fd;
    struct pollfd *client_poll;
    struct sockaddr_in *client_add;
    char *team;
    player_t *player;
    struct client_s *next;
    bool is_fully_connected;
} client_t;

#endif /* !CLIENT_H_ */
