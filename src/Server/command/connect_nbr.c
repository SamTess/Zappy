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

static int count_client_egg(client_t *creator, egg_t *current, char *team_name)
{
    int count = 0;

    if (creator)
        creator = creator->next;
    while (creator) {
        if (creator->client_id == current->creator_id &&
            creator->player && creator->player->team_name &&
            strcmp(creator->player->team_name, team_name) == 0) {
            count++;
            break;
        }
        creator = creator->next;
    }
    return count;
}

static int count_team_eggs(server_t *server, char *team_name)
{
    egg_t *current = server->eggs;
    client_t *creator;
    int count = 0;

    while (current) {
        creator = server->client;
        count += count_client_egg(creator, current, team_name);
        current = current->next;
    }
    return count;
}

static void format_response(int available_slots, client_t *client)
{
    size_t res_size = sprintf(NULL, "%d\n", available_slots);
    char *response = malloc(sizeof(char) * res_size);

    snprintf(response, res_size, "%d\n", available_slots);
    write_command_output(client->client_fd, response);
    free(response);
}

void connect_nbr(server_t *server, client_t *client, char *buffer)
{
    int current_team_clients;
    int team_eggs;
    int available_slots;

    (void)buffer;
    if (!client || !client->player || !client->player->team_name) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    current_team_clients = count_team_clients(server,
        client->player->team_name);
    team_eggs = count_team_eggs(server, client->player->team_name);
    available_slots = (server->parsed_info->client_nb + team_eggs)
        - current_team_clients;
    if (available_slots < 0)
        available_slots = 0;
    format_response(available_slots, client);
}
