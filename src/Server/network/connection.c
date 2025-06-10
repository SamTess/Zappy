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

static bool remove_head_client(server_t *server, int fd)
{
    client_t *current = server->client;
    int client_id;

    if (current->client_fd != fd)
        return false;
    client_id = current->client_id;
    if (current->client_fd == server->s_fd) {
        printf("Error: Attempting to remove server listening socket\n");
        return true;
    }
    server->client = current->next;
    free_node(current, server);
    server->nfds -= 1;
    printf("Client %d disconnected\n", client_id);
    return true;
}

static bool remove_other_client(server_t *server, int fd)
{
    client_t *current = server->client;
    client_t *prev = NULL;
    int client_id;

    while (current != NULL && current->client_fd != fd) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Error: Client with fd %d not found during removal\n", fd);
        return false;
    }
    client_id = current->client_id;
    prev->next = current->next;
    free_node(current, server);
    server->nfds -= 1;
    printf("Client %d disconnected\n", client_id);
    return true;
}

void remove_fd(server_t *server, int fd)
{
    if (server->client == NULL)
        return;
    if (remove_head_client(server, fd))
        return;
    remove_other_client(server, fd);
}

static client_t *init_new_client(int fd)
{
    client_t *new_c = calloc(1, sizeof(client_t));

    if (!new_c)
        server_err("New client allocation failed");
    new_c->client_poll = calloc(1, sizeof(struct pollfd));
    if (!new_c->client_poll)
        server_err("Poll fd struct allocation in new client struct failed");
    new_c->client_poll->fd = fd;
    new_c->client_poll->events = POLLIN;
    new_c->client_poll->revents = 0;
    new_c->next = NULL;
    new_c->client_fd = fd;
    new_c->client_add = NULL;
    new_c->client_id = -1;
    new_c->player = calloc(1, sizeof(player_t));
    new_c->is_fully_connected = false;
    if (new_c->player == NULL)
        server_err("Failed to allocate player");
    init_player(new_c->player, NULL);
    return new_c;
}

void add_fd(server_t *server, int fd)
{
    client_t *new_c = init_new_client(fd);
    client_t *current;
    int next_id = 0;

    if (server->client == NULL) {
        server->client = new_c;
        new_c->client_id = -1;
        return;
    }
    current = server->client;
    while (current->next != NULL) {
        if (current->client_id >= next_id) {
            next_id = current->client_id + 1;
        }
        current = current->next;
    }
    if (current->client_id >= next_id)
        next_id = current->client_id + 1;
    new_c->client_id = next_id;
    current->next = new_c;
}

static void init_server_socket(server_t *server, parsing_info_t *parsed_info)
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
    add_fd(server, server->s_fd);
}

static void copy_names(server_t *server, parsing_info_t *parsed_info)
{
    int i = 0;

    for (; parsed_info->names[i] != NULL; i++){
    }
    server->parsed_info->names = calloc(i + 1, sizeof(char *));
    if (server->parsed_info->names == NULL)
        server_err("Malloc failed for parsing info names copy into server");
    for (int j = 0; parsed_info->names[j] != NULL; j++){
        server->parsed_info->names[j] = strdup(parsed_info->names[j]);
    }
    server->parsed_info->names[i] = NULL;
}

void init_server_resources(server_t *server)
{
    server->total_resources = malloc(sizeof(int) * COUNT);
    server->current_resources = malloc(sizeof(int) * COUNT);
    if (server->total_resources == NULL || server->current_resources == NULL)
        server_err("Malloc failed for allocating resource counters");
    for (int i = 0; i < COUNT; i++) {
        server->total_resources[i] = 0;
        server->current_resources[i] = 0;
    }
}

static void init_server(server_t *server, parsing_info_t *parsed_info)
{
    server->nfds = 0;
    server->client = NULL;
    server->graphical_clients = NULL;
    server->s_fd = 0;
    server->serv_add = NULL;
    server->current_tick = 0;
    server->map = NULL;
    server->parsed_info = malloc(sizeof(parsing_info_t));
    if (server->parsed_info == NULL)
        server_err("Malloc failed for allocating parsed_info in server");
    server->parsed_info->port = parsed_info->port;
    server->parsed_info->width = parsed_info->width;
    server->parsed_info->height = parsed_info->height;
    server->parsed_info->client_nb = parsed_info->client_nb;
    server->parsed_info->frequence = parsed_info->frequence;
    server->eggs = NULL;
    init_server_resources(server);
    copy_names(server, parsed_info);
}

void create_server(server_t *server, parsing_info_t *parsed_info)
{
    init_server(server, parsed_info);
    init_server_socket(server, parsed_info);
}
