/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command_utils
*/
#include "../include/command.h"
#include "../include/parsing.h"
#include "../include/zappy.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

void cleanup_pending(player_t *player)
{
    if (!player || !player->pending_cmd)
        return;
    if (player->pending_cmd->args){
        free(player->pending_cmd->args);
        player->pending_cmd->args = NULL;
    }
    free(player->pending_cmd);
    player->pending_cmd = NULL;
}

void add_pending_cmd(zappy_client_t *player, game_t *game, char *buffer, int cmd_index, zappy_client_t *clients)
{
    command_data_t data = get_command_data();
    char **tmp = NULL;

    (void)clients;

    if (cmd_index == 9) {
        tmp = str_to_word_arr(buffer, " ");
        start_incantation(game, player, clients, tmp);
        return free_arr(tmp);
    }
    if (cmd_index == 10)
        command_pfk(game, player, clients);
    player->player->pending_cmd->args = strdup(buffer);
    player->player->pending_cmd->func = data.functions[cmd_index];
    if (data.times[cmd_index] > 0)
        player->player->busy_until =
            game->current_tick + data.times[cmd_index];
}

void cleanup_player_queue(player_t *player)
{
    if (!player || !player->command_queue)
        return;
    for (int i = 0; i < player->queue_size; i++) {
        if (player->command_queue[i]) {
            free(player->command_queue[i]);
            player->command_queue[i] = NULL;
        }
    }
    free(player->command_queue);
    player->command_queue = NULL;
    player->queue_size = 0;
}

static int check_socket_ready(struct pollfd *pfd)
{
    int poll_result;

    pfd->revents = 0;
    poll_result = poll(pfd, 1, 10);
    if (poll_result == -1)
        return -1;
    if (poll_result == 0 || !(pfd->revents & POLLOUT))
        return 0;
    return 1;
}

static int handle_write_result(ssize_t bytes_written, int *retry_count)
{
    if (bytes_written == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            (*retry_count)++;
            return 0;
        }
        perror("Write failed");
        return -1;
    }
    if (bytes_written == 0) {
        (*retry_count)++;
        return 0;
    }
    *retry_count = 0;
    return 1;
}

static int attempt_write(int client_fd, char *msg,
    ssize_t *total_written, ssize_t msg_len)
{
    ssize_t bytes_written;

    bytes_written = write(client_fd, msg + *total_written,
        msg_len - *total_written);
    if (bytes_written > 0)
        *total_written += bytes_written;
    return bytes_written;
}

static void write_with_poll(int client_fd, struct pollfd *pfd,
    char *msg, ssize_t msg_len)
{
    ssize_t total_written = 0;
    int retry_count = 0;
    int socket_status;
    int write_result;

    while (total_written < msg_len && retry_count < MAX_RETRY) {
        socket_status = check_socket_ready(pfd);
        if (socket_status == -1)
            return;
        if (socket_status == 0) {
            retry_count++;
            continue;
        }
        write_result = attempt_write(client_fd, msg, &total_written, msg_len);
        if (handle_write_result(write_result, &retry_count) == -1)
            return;
    }
    if (total_written < msg_len)
        perror("Failed to write full message");
}

static int validate_client_fd(int client_fd)
{
    if (fcntl(client_fd, F_GETFD) == -1) {
        perror("FD isn't up anymore");
        return 0;
    }
    return 1;
}

void write_command_output(int client_fd, char *msg)
{
    struct pollfd pfd;

    if (!validate_client_fd(client_fd))
        return;
    pfd.fd = client_fd;
    pfd.events = POLLOUT;
    write_with_poll(client_fd, &pfd, msg, strlen(msg));
}

void write_command_output_buffer(client_t *client, char *msg)
{
    if (!client || !validate_client_fd(client->client_fd))
        return;
    if (add_string_to_write_buffer(&client->write_buffer, msg) == -1)
        return write_command_output(client->client_fd, msg);
    client->need_write = true;
}
