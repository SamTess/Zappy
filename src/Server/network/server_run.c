/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** server_run
*/
#include "../include/server.h"
#include "../include/command.h"
#include "../include/player.h"
#include "../include/pending_cmd_utils.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

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
    if (new_client != NULL)
        write_command_output(new_client->client_fd, "WELCOME\n");
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

static void setup_poll_manager(poll_manager_t *poll_mana, int size)
{
    if (!poll_mana->fds) {
        poll_mana->fds = malloc(size * sizeof(struct pollfd));
        if (!poll_mana->fds)
            server_err("Poll_mana alloc failed\n");
        poll_mana->capacity = size;
    }
    if (size > poll_mana->capacity) {
        poll_mana->fds = realloc(poll_mana->fds, size * sizeof(struct pollfd));
        if (!poll_mana->fds)
            server_err("Poll_mana realloc failed\n");
        poll_mana->capacity = size;
    }
    poll_mana->needs_rebuild = true;
}

static void fill_poll_array(server_t *server, poll_manager_t *poll_mana)
{
    client_t *current = server->client;
    int size = server->nfds + 1;

    poll_mana->fds[0] = *(current->client_poll);
    current = current->next;
    for (int i = 1; i < size && current != NULL; i++) {
        poll_mana->fds[i] = *(current->client_poll);
        current = current->next;
    }
    poll_mana->needs_rebuild = false;
}

static void poll_client(server_t *server, poll_manager_t *poll_mana)
{
    client_t *current = server->client;
    int size = server->nfds + 1;

    current->client_poll->revents = poll_mana->fds[0].revents;
    check_new_connection(server);
    current = current->next;
    for (int i = 1; i < size && current != NULL; i++) {
        current->client_poll->revents = poll_mana->fds[i].revents;
        current = current->next;
    }
    if (server->nfds > 0)
        check_client_message(server);
}

static void handle_game_tick(server_t *server)
{
    static struct timeval last_tick = {0, 0};
    struct timeval current_time;
    long time_diff;

    gettimeofday(&current_time, NULL);
    time_diff = (current_time.tv_sec - last_tick.tv_sec) * 1000 +
        (current_time.tv_usec - last_tick.tv_usec) / 1000;
    if (time_diff >= (1000 / server->parsed_info->frequence)) {
        update_game_tick(server);
        last_tick = current_time;
    }
}

void check_client(server_t *server)
{
    int size = server->nfds + 1;

    setup_poll_manager(server->poll_manager, size);
    handle_game_tick(server);
    if (server->poll_manager->needs_rebuild)
        fill_poll_array(server, server->poll_manager);
    if (poll(server->poll_manager->fds, size, 10) > 0)
        poll_client(server, server->poll_manager);
}
