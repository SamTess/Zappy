/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** zappy
*/

#ifndef ZAPPY_H_
    #define ZAPPY_H_
    #include "server.h"
    #include "game.h"

enum client_type_e {
    GRAPHICAL,
    AI
};

typedef struct zappy_client_s {
    client_t *client;
    player_t *player;
    bool is_fully_connected;
    enum client_type_e type;
    struct zappy_client_s *next;
} zappy_client_t;

typedef struct zappy_s {
    server_t *server;
    game_t *game;
    zappy_client_t *clients;
    int next_client_id;
    bool should_run;
} zappy_t;

void zappy_create(parsing_info_t *config, zappy_t *zappy);
#endif /* !ZAPPY_H_ */
