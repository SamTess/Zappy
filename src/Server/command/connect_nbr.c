/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** connect_nbr
*/
#include "../include/command.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int count_team_eggs(server_t *server, char *team_name)
{
    egg_t *current = server->eggs;
    egg_t *next;
    int count = 0;

    while (current) {
        next = current->next;
        if (current->team_name && strcmp(current->team_name, team_name) == 0)
            count++;
        current = next;
    }
    return count;
}

static int count_team_players(server_t *server, char *team_name)
{
    client_t *current = server->client;
    int count = 0;

    while (current) {
        if (current->player && current->player->team_name &&
            strcmp(current->player->team_name, team_name) == 0)
            count++;
        current = current->next;
    }
    return count;
}

static void format_response(int available_slots, client_t *client)
{
    size_t res_size = snprintf(NULL, 0, "%d\n", available_slots) + 1;
    char *response = malloc(sizeof(char) * res_size);

    snprintf(response, res_size, "%d\n", available_slots);
    write_command_output(client->client_fd, response);
    free(response);
}

int connect_nbr_srv(server_t *server, char *team)
{
    int team_eggs = count_team_eggs(server, team);
    int team_players = count_team_players(server, team);
    int max_clients = server->parsed_info->client_nb;
    int available_slots = max_clients - (team_players + team_eggs);

    if (available_slots < 0)
        available_slots = 0;
    return available_slots;
}

void connect_nbr(server_t *server, client_t *client, char **buffer)
{
    int available_slots;

    if (!client || !client->player || !client->player->team_name ||
        arr_len(buffer) != 1) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    available_slots = connect_nbr_srv(server, client->player->team_name);
    format_response(available_slots, client);
}
