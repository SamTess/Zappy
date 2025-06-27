/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** connection
*/
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <linux/limits.h>
#include "../include/server.h"
#include "../include/parsing.h"
#include "../include/command.h"

static bool remove_head_client(server_t *server,
    zappy_client_t **clients, int fd)
{
    zappy_client_t *current = *clients;

    if (current->client->client_fd != fd)
        return false;
    if (current->client->client_fd == server->s_fd)
        return true;
    *clients = current->next;
    free_node(current);
    server->nfds -= 1;
    return true;
}

static bool remove_other_client(server_t *server,
    zappy_client_t *clients, int fd)
{
    zappy_client_t *current = clients;
    zappy_client_t *prev = NULL;

    while (current != NULL && current->client->client_fd != fd) {
        prev = current;
        current = current->next;
    }
    if (current == NULL)
        return false;
    prev->next = current->next;
    free_node(current);
    server->nfds -= 1;
    return true;
}

void remove_fd(server_t *server, zappy_client_t **clients, int fd)
{
    if (*clients == NULL)
        return;
    if (remove_head_client(server, clients, fd))
        return;
    remove_other_client(server, *clients, fd);
    close(fd);
}

static zappy_client_t *init_new_client(int fd)
{
    zappy_client_t *new_c = calloc(1, sizeof(zappy_client_t));

    if (!new_c)
        return NULL;
    if (init_zappy_client_struct(new_c) == -1)
        return NULL;
    new_c->client->client_poll->fd = fd;
    new_c->client->client_poll->events = POLLIN;
    new_c->client->client_poll->revents = 0;
    new_c->next = NULL;
    new_c->client->client_fd = fd;
    new_c->client->client_add = NULL;
    new_c->client->client_id = -1;
    new_c->player = calloc(1, sizeof(player_t));
    if (new_c->player == NULL)
        return NULL;
    new_c->is_fully_connected = false;
    init_struct(new_c);
    return new_c;
}

void add_fd(zappy_client_t **clients, int fd)
{
    static int next_id = 0;
    zappy_client_t *new_c = init_new_client(fd);
    zappy_client_t *current;

    if (*clients == NULL) {
        if (new_c == NULL)
            server_err("Server polling client init failed");
        new_c->client->client_id = -1;
        *clients = new_c;
        return;
    }
    if (new_c == NULL)
        return;
    current = *clients;
    while (current->next != NULL) {
        current = current->next;
    }
    new_c->client->client_id = next_id;
    next_id++;
    current->next = new_c;
}

static void init_server_socket(server_t *server,
    parsing_info_t *parsed_info, zappy_client_t **clients)
{
    int opt = 1;

    server->s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->s_fd == -1)
        server_err("Socket creation failed");
    if (setsockopt(server->s_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        server_err("setsockopt failed");
    server->serv_add = calloc(1, sizeof(struct sockaddr_in));
    if (server->serv_add == NULL)
        server_err("Failed to allocate server address");
    server->serv_add->sin_family = AF_INET;
    server->serv_add->sin_addr.s_addr = INADDR_ANY;
    server->serv_add->sin_port = htons(parsed_info->port);
    if (bind(server->s_fd, (struct sockaddr *)server->serv_add,
        sizeof(struct sockaddr_in)) < 0)
        server_err("Connection bind failed");
    if (listen(server->s_fd, 1000) < 0)
        server_err("Connection listen failed");
    add_fd(clients, server->s_fd);
}

static void init_server(server_t *server)
{
    server->nfds = 0;
    server->s_fd = 0;
    server->serv_add = NULL;
    server->should_run = true;
    server->poll_manager = calloc(1, sizeof(poll_manager_t));
}

void create_server(server_t *server, parsing_info_t *parsed_info,
    zappy_client_t **clients)
{
    init_server(server);
    init_server_socket(server, parsed_info, clients);
}
