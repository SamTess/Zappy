/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command
*/
#include "../include/command.h"
#include "../include/server.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int check_disconnect(int bytes_read, client_t *user,
    server_t *server, char *buffer)
{
    if (bytes_read <= 0) {
        if (bytes_read == 0){
            cleanup_client(user);
            remove_fd(server, user->client_poll->fd);
            free(buffer);
            return 1;
        }
        return 1;
    }
    return 0;
}

static int manage_buffer(char **buffer, int *b_size, char byte)
{
    char *new_buffer = realloc(*buffer, (*b_size + 2) * sizeof(char));

    if (!new_buffer){
        perror("Realloc failed");
        exit(84);
    }
    *buffer = new_buffer;
    (*buffer)[*b_size] = byte;
    *b_size += 1;
    if (*b_size >= 2 && (*buffer)[*b_size - 2] == '\r' &&
        (*buffer)[*b_size - 1] == '\n')
        return 1;
    return 0;
}

static int check_buffer_size(int b_size, char *buffer, client_t *user)
{
    if (b_size > 8192) {
        write(user->client_fd, "trop long", strlen("trop long"));
        free(buffer);
        return 1;
    }
    return 0;
}

static command_data_t get_command_data(void)
{
        "Inventory", "Look", "Eject", "Take", "Set", "Connect_nbr", NULL};
    static void (*comm_func[])(server_t *, client_t *, char *) =
        {forward, right, left, inventory, look, eject, take_object,
            set_object, connect_nbr, NULL};
    static int comm_times[] = {7, 7, 7, 1, 7, 7, 7, 7, 0};
    command_data_t data = {comm_char, comm_func, comm_times};

    return data;
}

static bool execute_if_free(server_t *server, client_t *user,
    char *buffer, int cmd_index)
{
    command_data_t data = get_command_data();

    if (user->player->busy_until <= server->current_tick) {
        data.functions[cmd_index](server, user, buffer);
        if (data.times[cmd_index] > 0)
            user->player->busy_until =
                server->current_tick + data.times[cmd_index];
        return true;
    } else {
        if (user->player->queue_size < 10) {
            add_to_command_queue(user, buffer);
            return true;
        } else
            return true;
    }
}

static bool find_and_execute(server_t *server, client_t *user, char *buffer)
{
    command_data_t data = get_command_data();

    for (int i = 0; data.commands[i] != NULL; i++) {
        if (strncmp(buffer, data.commands[i], strlen(data.commands[i])) == 0)
            return execute_if_free(server, user, buffer, i);
    }
    return false;
}

void execute_com(server_t *server, client_t *user, char *buffer)
{
    if (!user || !user->player)
        return;
    if (!find_and_execute(server, user, buffer))
        write_command_output(user->client_fd, "ko\n");
}

void get_message(server_t *server, client_t *user)
{
    char *buffer = calloc(2, sizeof(char));
    int b_size = 0;
    char byte;
    int bytes_read;

    while (1){
        bytes_read = read(user->client_poll->fd, &byte, 1);
        if (check_disconnect(bytes_read, user, server, buffer) == 1)
            return;
        if (manage_buffer(&buffer, &b_size, byte) == 1)
            break;
        if (check_buffer_size(b_size, buffer, user) == 1)
            return;
    }
    buffer[b_size] = '\0';
    execute_com(server, user, buffer);
    free(buffer);
}
