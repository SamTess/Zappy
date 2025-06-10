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
    int team_eggs;

    team_eggs = count_team_eggs(server, team);
    if (team_eggs < 0)
        team_eggs = -1;
    return team_eggs;
}

void connect_nbr(server_t *server, client_t *client, char *buffer)
{
    int team_eggs;

    (void)buffer;
    if (!client || !client->player || !client->player->team_name) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    team_eggs = count_team_eggs(server, client->player->team_name);
    if (team_eggs < 0)
        team_eggs = 0;
    format_response(team_eggs, client);
}
