/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** set_object
*/

#include "../include/command.h"
#include <stdio.h>

void set_object(server_t *server, client_t *client, char *buffer)
{
    char *resource_name;
    resource_type_t resource_type = determine_type(resource_name);

        if (strlen(buffer) <= 4)
        return write_command_output(client->client_fd, "ko\n");
    resource_name = buffer + 4;
    if (client == NULL || client->player == NULL || resource_type == COUNT)
        return write_command_output(client->client_fd, "ko\n");
    if (how_many_in_inventory(client->player, resource_type) <= 0)
        return write_command_output(client->client_fd, "ko\n");
    remove_item_from_inventory(client->player, resource_type, 1);
    server->map[client->player->pos_y]
        [client->player->pos_x].resources[resource_type]++;
    write_command_output(client->client_fd, "ok\n");
}
