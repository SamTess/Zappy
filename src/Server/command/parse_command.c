/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command
*/
#include "../include/command.h"
#include "../include/server.h"
#include "../include/pending_cmd.h"
#include "../include/circular_buffer.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int check_disconnect(int bytes_read, client_t *user, server_t *server)
{
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            cleanup_client(user);
            remove_fd(server, user->client_poll->fd);
            return 1;
        }
        return 1;
    }
    return 0;
}

command_data_t get_command_data(void)
{
    static const char *comm_char[] = {"Forward", "Right", "Left",
        "Inventory", "Look", "Eject", "Connect_nbr", "Take", "Set",
        "Incantation", "Fork", "Broadcast", NULL};
    static void (*comm_func[])(server_t *, client_t *, char *) =
        {forward, right, left, inventory, look, eject,
        connect_nbr, take_object, set_object, start_incantation,
        fork_c, broadcast, NULL};
    static int comm_times[] = {7, 7, 7, 1, 7, 7, 0, 7, 7, 300, 42, 7};
    command_data_t data = {comm_char, comm_func, comm_times};

    return data;
}

static bool execute_if_free(server_t *server, client_t *user,
    char *buffer, int cmd_index)
{
    command_data_t data = get_command_data();

    if (user->player->busy_until <= server->current_tick) {
        user->player->pending_cmd->args = strdup(buffer);
        user->player->pending_cmd->func = data.functions[cmd_index];
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

static void check_command(circular_buffer_t *temp_buffer, int cmd_length,
    server_t *server, client_t *user)
{
    char *command;

    command = extract_command(temp_buffer, cmd_length);
    if (command) {
        execute_com(server, user, command);
        free(command);
    }
}

void get_message(server_t *server, client_t *user)
{
    circular_buffer_t temp_buffer;
    char byte;
    int bytes_read;
    int cmd_length;

    init_circular_buffer(&temp_buffer);
    while (1) {
        bytes_read = read(user->client_poll->fd, &byte, 1);
        if (check_disconnect(bytes_read, user, server) == 1)
            return;
        if (add_to_circular_buffer(&temp_buffer, byte) == -1) {
            write(user->client_fd, "trop long", strlen("trop long"));
            return;
        }
        cmd_length = find_command_end(&temp_buffer);
        if (cmd_length > 0) {
            check_command(&temp_buffer, cmd_length, server, user);
            break;
        }
    }
}
