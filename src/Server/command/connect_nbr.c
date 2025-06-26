/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** connect_nbr
*/
#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int count_team_eggs(game_t *game, char *team_name)
{
    egg_t *current = game->eggs;
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

static void format_response(int available_slots, zappy_client_t *client)
{
    size_t res_size = snprintf(NULL, 0, "%d\n", available_slots) + 1;
    char *response = malloc(sizeof(char) * res_size);

    if (!response || !client || !client->client)
        return;
    snprintf(response, res_size, "%d\n", available_slots);
    write_command_output_buffer(client->client, response);
    free(response);
}

int connect_nbr_srv(game_t *game, char *team)
{
    int team_eggs = count_team_eggs(game, team);
    int available_slots = team_eggs;

    if (available_slots < 0)
        available_slots = 0;
    return available_slots;
}

void connect_nbr(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    int available_slots;

    (void)clients;
    if (!game || !client || !client->client || !client->player ||
        !client->player->team_name || arr_len(buffer) != 1) {
        write_command_output_buffer(client->client, "ko\n");
        return;
    }
    available_slots = connect_nbr_srv(game, client->player->team_name);
    format_response(available_slots, client);
}
