/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** take_object
*/

#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

resource_type_t determine_type(char *resource_string)
{
    if (strncmp(resource_string, "food", 4) == 0)
        return FOOD;
    if (strncmp(resource_string, "linemate", 8) == 0)
        return LINEMATE;
    if (strncmp(resource_string, "deraumere", 9) == 0)
        return DERAUMERE;
    if (strncmp(resource_string, "sibur", 5) == 0)
        return SIBUR;
    if (strncmp(resource_string, "mendiane", 8) == 0)
        return MENDIANE;
    if (strncmp(resource_string, "phiras", 6) == 0)
        return PHIRAS;
    if (strncmp(resource_string, "thystame", 8) == 0)
        return THYSTAME;
    return COUNT;
}

static void update_resources(server_t *server, client_t *client,
    resource_type_t type)
{
    server->map[client->player->pos_y]
    [client->player->pos_x].resources[type]--;
    server->current_resources[type]--;
}

void take_object(server_t *server, client_t *client, char **buffer)
{
    resource_type_t type;

    if (!server || !client || !client->player || arr_len(buffer) != 2)
        return write_command_output(client->client_fd, "ko\n");
    type = determine_type(buffer[1]);
    if (type == COUNT)
        return write_command_output(client->client_fd, "ko\n");
    if (server->map[client->player->pos_y]
        [client->player->pos_x].resources[type] > 0) {
        update_resources(server, client, type);
        add_item_to_inventory(client->player, type, 1);
        command_pgt(server, client, type);
        send_bct_to_all_graphical_clients(server, client->player->pos_x,
            client->player->pos_y);
        send_pin_to_all(server, client);
        write_command_output(client->client_fd, "ok\n");
    } else
        write_command_output(client->client_fd, "ko\n");
}
