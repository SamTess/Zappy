/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** scheduled_command
*/
#ifndef SCHEDULED_COMMAND_H_
    #define SCHEDULED_COMMAND_H_

struct server_s;
typedef struct server_s server_t;
struct client_s;
typedef struct client_s client_t;

typedef void (*command_func_t)(server_t *, client_t *, char *);

typedef struct scheduled_command_s {
    command_func_t func;
    char *args;
    int execute_at_tick;
} scheduled_command_t;

#endif // SCHEDULED_COMMAND_H_
