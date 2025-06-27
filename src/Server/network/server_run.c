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
#include "../include/zappy.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

static void new_connection(server_t *server, zappy_client_t **clients)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd;
    zappy_client_t *new_client;

    memset(&client_addr, 0, sizeof(client_addr));
    client_fd = accept(server->s_fd, (struct sockaddr *)&client_addr,
        &addr_len);
    if (client_fd < 0)
        return;
    add_fd(clients, client_fd);
    server->nfds += 1;
    new_client = find_client_by_socket(*clients, server, client_fd);
    if (new_client != NULL && new_client->client != NULL)
        write_command_output(new_client->client->client_fd, "WELCOME\n");
}

static void check_new_connection(server_t *server, zappy_client_t **clients)
{
    if ((*clients)->client->client_poll->revents & POLLIN)
        new_connection(server, clients);
}

static void pollin_pollout_client(zappy_client_t *temp,
    zappy_t *zappy, int temp_fd)
{
    if (temp->client->client_poll->revents & POLLIN)
        get_message(zappy->server, temp, zappy->clients, zappy->game);
    if (find_client_by_socket(zappy->clients, zappy->server, temp_fd) != NULL
        && temp->client->client_poll->revents & POLLOUT
        && temp->client->need_write)
        flush_client_write_buffer(temp->client);
}

static void check_client_message(zappy_t *zappy)
{
    zappy_client_t *temp = zappy->clients;
    zappy_client_t *next = NULL;
    int temp_fd;

    if (temp != NULL)
        temp = temp->next;
    while (temp != NULL) {
        next = temp->next;
        temp_fd = temp->client->client_fd;
        if (temp->client->client_poll != NULL
            && temp->client->client_poll->revents != 0)
            pollin_pollout_client(temp, zappy, temp_fd);
        temp = next;
    }
}

static int setup_poll_manager(poll_manager_t *poll_mana, int size)
{
    if (!poll_mana->fds) {
        poll_mana->fds = malloc(size * sizeof(struct pollfd));
        if (!poll_mana->fds)
            return -1;
        poll_mana->capacity = size;
    }
    if (size > poll_mana->capacity) {
        poll_mana->fds = realloc(poll_mana->fds, size * sizeof(struct pollfd));
        if (!poll_mana->fds)
            return -1;
        poll_mana->capacity = size;
    }
    poll_mana->needs_rebuild = true;
    return 0;
}

static void smart_polling(zappy_client_t *current, poll_manager_t *poll_mana,
    game_t *game, int i)
{
    if (current->type == GRAPHICAL) {
        poll_mana->fds[i].events |= POLLIN;
    } else if (current->player &&
        current->player->busy_until > game->current_tick &&
        current->player->queue_size >= 10) {
        poll_mana->fds[i].events = 0;
    } else
        poll_mana->fds[i].events |= POLLIN;
    if (current->client->need_write)
        poll_mana->fds[i].events |= POLLOUT;
}

static void fill_poll_array(server_t *server, game_t *game,
    zappy_client_t **clients, poll_manager_t *poll_mana)
{
    zappy_client_t *current = *clients;
    int size = server->nfds + 1;

    poll_mana->fds[0] = *(current->client->client_poll);
    current = current->next;
    for (int i = 1; i < size && current != NULL; i++) {
        poll_mana->fds[i] = *(current->client->client_poll);
        smart_polling(current, poll_mana, game, i);
        current = current->next;
    }
    poll_mana->needs_rebuild = false;
}

static void poll_client(zappy_t *zappy)
{
    zappy_client_t *current = zappy->clients;
    int size = zappy->server->nfds + 1;

    current->client->client_poll->revents =
        zappy->server->poll_manager->fds[0].revents;
    check_new_connection(zappy->server, &zappy->clients);
    current = current->next;
    for (int i = 1; i < size && current != NULL; i++) {
        current->client->client_poll->revents =
            zappy->server->poll_manager->fds[i].revents;
        current = current->next;
    }
    if (zappy->server->nfds > 0)
        check_client_message(zappy);
}

static void handle_game_tick(game_t *game, server_t *server,
    zappy_client_t *clients)
{
    static struct timeval last_tick = {0, 0};
    struct timeval current_time;
    long time_diff;

    gettimeofday(&current_time, NULL);
    time_diff = (current_time.tv_sec - last_tick.tv_sec) * 1000 +
        (current_time.tv_usec - last_tick.tv_usec) / 1000;
    if (time_diff >= (1000 / game->parsed_info->frequence)) {
        update_game_tick(game, server, clients);
        last_tick = current_time;
    }
}

void check_client(zappy_t *zappy)
{
    int size = zappy->server->nfds + 1;
    int temp = 0;

    temp = setup_poll_manager(zappy->server->poll_manager, size);
    if (temp == -1 || !zappy->server->poll_manager->fds)
        return;
    handle_game_tick(zappy->game, zappy->server, zappy->clients);
    if (zappy->server->poll_manager->needs_rebuild)
        fill_poll_array(zappy->server, zappy->game,
            &zappy->clients, zappy->server->poll_manager);
    if (poll(zappy->server->poll_manager->fds, size, 10) > 0)
        poll_client(zappy);
}
