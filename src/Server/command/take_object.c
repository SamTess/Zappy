/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** take_object
*/

#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

ResourceType_t determine_type(char *cut_string)
{
    if (strncmp(cut_string, "food", 4) == 0)
        return FOOD;
    if (strncmp(cut_string, "linemate", 8) == 0)
        return LINEMATE;
    if (strncmp(cut_string, "deraumere", 9) == 0)
        return DERAUMERE;
    if (strncmp(cut_string, "sibur", 5) == 0)
        return SIBUR;
    if (strncmp(cut_string, "mendiane", 8) == 0)
        return MENDIANE;
    if (strncmp(cut_string, "phiras", 6) == 0)
        return PHIRAS;
    return COUNT;
}

void take_object(server_t *server, client_t *client, char *buffer)
{
    char *cut_string = buffer + 5;
    ResourceType_t resource_type = determine_type(cut_string);

    if (client == NULL || client->player == NULL) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    if (resource_type == COUNT) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    if (server->map[client->player->pos_y][client->player->pos_x].resources[resource_type] > 0) {
        server->map[client->player->pos_y][client->player->pos_x].resources[resource_type]--;
        add_item_to_inventory(client->player, resource_type, 1);
        write_command_output(client->client_fd, "ok\n");
    } else {
        write_command_output(client->client_fd, "ko\n");
    }
}