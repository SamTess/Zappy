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
    #include "circular_buffer.h"

typedef struct client_s {
    int client_id;
    int client_fd;
    struct pollfd *client_poll;
    struct sockaddr_in *client_add;
    circular_buffer_t read_buffer;
    circular_buffer_t write_buffer;
    bool need_write;
} client_t;

void send_message_to_all_graphic(zappy_client_t *clients, char *message);
void send_map_info_to_one_client(game_t *game, zappy_client_t *clients, zappy_client_t *graphic_client);
void write_command_output_buffer(client_t *client, char *message);

#endif /* !CLIENT_H_ */
