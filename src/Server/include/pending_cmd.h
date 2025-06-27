/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pdniingmd
*/
#ifndef PENDING_CMD_H_
    #define PENDING_CMD_H_

struct game_s;
typedef struct game_s game_t;
struct zappy_client_s;
typedef struct zappy_client_s zappy_client_t;

typedef void (*cmd_func_t)(game_t *, zappy_client_t *,
    zappy_client_t *, char **);

typedef struct pending_cmd_s {
    cmd_func_t func;
    char *args;
} pending_cmd_t;

#endif /* !PENDING_CMD_H_ */
