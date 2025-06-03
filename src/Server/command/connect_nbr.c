/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** connect_nbr
*/
#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int count_team_clients(server_t *server, char *team_name)
{
    client_t *current = server->client;
    int count = 0;

    if (!current)
        return 0;
    current = current->next;
    while (current) {
        if (current->player && current->player->team_name &&
            strcmp(current->player->team_name, team_name) == 0) {
            count++;
        }
        current = current->next;
    }
    return count;
}

void connect_nbr(server_t *server, client_t *client, char *buffer)
{
    int current_team_clients;
    int available_slots;
    char response[16];

    (void)buffer;
    if (!client || !client->player || !client->player->team_name) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    current_team_clients = count_team_clients(server,
        client->player->team_name);
    available_slots = server->parsed_info->client_nb - current_team_clients;
    if (available_slots < 0)
        available_slots = 0;
    snprintf(response, sizeof(response), "%d\n", available_slots);
    write_command_output(client->client_fd, response);
}
