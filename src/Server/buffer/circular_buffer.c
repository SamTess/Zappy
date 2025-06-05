/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** circular_buffer
*/
#include "../include/server.h"
#include "../include/circular_buffer.h"
#include <stdlib.h>

void init_circular_buffer(circular_buffer_t *cb)
{
    cb->start = 0;
    cb->end = 0;
    cb->count = 0;
}

int add_to_circular_buffer(circular_buffer_t *cb, char byte)
{
    if (cb->count >= BUFFER_SIZE - 1)
        return -1;
    cb->buffer[cb->end] = byte;
    cb->end = (cb->end + 1) % BUFFER_SIZE;
    cb->count++;
    return 0;
}

int find_command_end(circular_buffer_t *cb)
{
    int pos = 0;
    int next_pos = 0;

    if (cb->count < 2)
        return -1;
    pos = cb->start;
    for (int i = 0; i < cb->count - 1; i++) {
        next_pos = (pos + 1) % BUFFER_SIZE;
        if (cb->buffer[pos] == '\r' && cb->buffer[next_pos] == '\n')
            return i + 2;
        pos = next_pos;
    }
    return -1;
}

char *extract_command(circular_buffer_t *cb, int cmd_length)
{
    char *command = malloc(cmd_length + 1);

    if (!command)
        return NULL;
    for (int i = 0; i < cmd_length; i++) {
        command[i] = cb->buffer[cb->start];
        cb->start = (cb->start + 1) % BUFFER_SIZE;
        cb->count--;
    }
    command[cmd_length] = '\0';
    return command;
}
