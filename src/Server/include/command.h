/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command
*/

#ifndef COMMAND_H_
    #define COMMAND_H_
    #include "server.h"

typedef struct command_data_s {
    const char **commands;
    void (**functions)(server_t *, client_t *, char *);
    int *times;
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

#endif /* !COMMAND_H_ */
