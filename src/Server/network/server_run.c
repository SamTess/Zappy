/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server_run
*/
#include "../include/server.h"
#include "../include/command.h"
#include "../include/player.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

static bool is_client_valid(server_t *server, client_t *target)
{
    client_t *current = server->client;

    while (current != NULL) {
        if (current == target)
            return true;
        current = current->next;
    }
    return false;
}

static void tick_check(server_t *server, client_t *current)
{
    if (current != NULL && current->player != NULL)
        check_player_starvation(server, current);
    if (is_client_valid(server, current) && current != NULL &&
        current->player != NULL)
        finish_incantation(server, current);
}

void update_game_tick(server_t *server)
{
    client_t *current = server->client;
    client_t *next;

    server->current_tick++;
    if (current != NULL)
        current = current->next;
    while (current != NULL) {
        next = current->next;
        tick_check(server, current);
        if (is_client_valid(server, current) && current->player &&
            current->player->busy_until <= server->current_tick &&
            current->player->queue_size > 0)
            process_next_queued_command(server, current);
        current = next;
    }
}

client_t *find_client_by_socket(server_t *server, int socket_fd)
{
    client_t *temp = server->client;

    if (temp && temp->client_fd == server->s_fd)
        temp = temp->next;
    while (temp) {
        if (temp->client_fd == socket_fd)
            return temp;
        temp = temp->next;
    }
    printf("Error: Could not find client for socket %d\n", socket_fd);
    return NULL;
}

static void new_connection(server_t *server)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];
    int client_fd;
    client_t *new_client;

    memset(&client_addr, 0, sizeof(client_addr));
    client_fd = accept(server->s_fd, (struct sockaddr *)&client_addr,
        &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(84);
    }
    add_fd(server, client_fd);
    server->nfds += 1;
    new_client = find_client_by_socket(server, client_fd);
    if (new_client != NULL){
        init_new_player_pos(server, new_client);
        write_command_output(new_client->client_fd, "WELCOME\n");
        print_co(client_ip, &client_addr, new_client);
    }
}

static void check_new_connection(server_t *server)
{
    if (server->client->client_poll->revents & POLLIN)
        new_connection(server);
}

static void check_client_message(server_t *server)
{
    client_t *temp = server->client;
    client_t *next = NULL;

    if (temp != NULL)
        temp = temp->next;
    while (temp != NULL) {
        next = temp->next;
        if (temp->client_poll != NULL && temp->client_poll->revents != 0
            && (temp->client_poll->revents & POLLIN))
                get_message(server, temp);
        temp = next;
    }
}

static void poll_fds(server_t *server, client_t *current, int nfds,
    struct pollfd *fds)
{
    server->client->client_poll->revents = fds[0].revents;
    check_new_connection(server);
    current = server->client->next;
    for (int i = 1; i < nfds && current != NULL; i++) {
        current->client_poll->revents = fds[i].revents;
        current = current->next;
    }
    if (server->nfds > 0)
        check_client_message(server);
}

static void helper_function(server_t *server, int nfds, struct pollfd *fds)
{
    client_t *current = server->client;
    int i = 0;

    fds[0] = *(current->client_poll);
    current = current->next;
    for (i = 1; i < nfds && current != NULL; i++) {
        fds[i] = *(current->client_poll);
        current = current->next;
    }
    if (poll(fds, nfds, 10) > 0)
        poll_fds(server, current, nfds, fds);
}

void check_client(server_t *server)
{
    struct pollfd *fds = calloc((server->nfds + 1), sizeof(struct pollfd));
    static struct timeval last_tick = {0, 0};
    struct timeval current_time;
    long time_diff;

    if (!fds) {
        perror("Poll array allocation failed");
        exit(84);
    }
    gettimeofday(&current_time, NULL);
    time_diff = (current_time.tv_sec - last_tick.tv_sec) * 1000 +
        (current_time.tv_usec - last_tick.tv_usec) / 1000;
    if (time_diff >= (1000 / server->parsed_info->frequence)) {
        update_game_tick(server);
        last_tick = current_time;
    }
    fds[0] = *(server->client->client_poll);
    helper_function(server, server->nfds + 1, fds);
    free(fds);
}
