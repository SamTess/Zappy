/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pdniingmd
*/
#ifndef PENDING_CMD_H_
    #define PENDING_CMD_H_

struct server_s;
typedef struct server_s server_t;
struct client_s;
typedef struct client_s client_t;

typedef void (*cmd_func_t)(server_t *, client_t *, char *);

typedef struct pending_cmd_s {
    cmd_func_t func;
    char *args;
} pending_cmd_t;

#endif /* !PENDING_CMD_H_ */
