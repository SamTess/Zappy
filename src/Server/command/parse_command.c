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

static void execute_com(server_t *server, client_t *user, char *buffer,
    bool found)
{
    const char *comm_char[] = {};
    void *(*comm_func[])(server_t *, client_t *, char *) = {};

    for (int i = 0; comm_char[i] != NULL; i++){
        if (strncmp(buffer, comm_char[i], 4) == 0){
            comm_func[i](server, user, buffer);
            found = true;
        }
    }
    if (!found)
        write(user->client_fd, "Unknown Command", strlen("Unknown Command"));
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
    execute_com(server, user, buffer, false);
    free(buffer);
}
