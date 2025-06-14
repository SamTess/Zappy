/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** graphical_client
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void add_to_graphical_list(server_t *server, graphical_client_t *new)
{
    graphical_client_t *current = NULL;

    if (server->graphical_clients == NULL) {
        server->graphical_clients = new;
    } else {
        current = server->graphical_clients;
        while (current->next != NULL)
            current = current->next;
        current->next = new;
    }
    new->next = NULL;
}

void add_graphic_client(server_t *server, client_t *client)
{
    graphical_client_t *new_ref = malloc(sizeof(graphical_client_t));

    if (!new_ref)
        return;
    new_ref->client = client;
    new_ref->next = server->graphical_clients;
    add_to_graphical_list(server, new_ref);
}

static bool remove_graphical_node(server_t *server, client_t *client,
    graphical_client_t **current, graphical_client_t **prev)
{
    if ((*current)->client == client) {
        if (*prev == NULL)
            server->graphical_clients = (*current)->next;
        else
            (*prev)->next = (*current)->next;
        free(*current);
        return true;
    }
    *prev = *current;
    *current = (*current)->next;
    return false;
}

void remove_graphic_client(server_t *server, client_t *client)
{
    graphical_client_t *current = server->graphical_clients;
    graphical_client_t *prev = NULL;

    if (!server || !client || !server->graphical_clients)
        return;
    while (current != NULL) {
        if (remove_graphical_node(server, client, &current, &prev))
            return;
    }
    remove_fd(server, client->client_fd);
}

void send_message_to_all_graphic(server_t *server, char *message)
{
    graphical_client_t *current = server->graphical_clients;

    while (current) {
        if (current->client && current->client->client_fd != -1)
            write_command_output(current->client->client_fd, message);
        current = current->next;
    }
}

void send_map_info_to_one_client(server_t *server, client_t *client)
{
    char **tmp;

    if (!server || !client)
        return;
    tmp = malloc(sizeof(char *) * 2);
    if (!tmp)
        return;
    tmp[0] = strdup("sgt\n");
    tmp[1] = NULL;
    send_msz_command(server, client);
    command_sgt(server, client, tmp);
    send_tile_content_to_one_client(server, client);
    send_tna_command(server, client);
    send_all_player_info_to_one_client(server, client);
    send_enw_command_start(server);
    free_arr(tmp);
}
