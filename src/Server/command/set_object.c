/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** set_object
*/

#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <string.h>

void set_object(server_t *server, client_t *client, char *buffer)
{
    char *resource_name;
    resource_type_t resource_type;

        if (strlen(buffer) <= 4)
        return write_command_output(client->client_fd, "ko\n");
    resource_name = buffer + 4;
    resource_type = determine_type(resource_name);
    if (client == NULL || client->player == NULL || resource_type == COUNT)
        return write_command_output(client->client_fd, "ko\n");
    if (how_many_in_inventory(client->player, resource_type) <= 0)
        return write_command_output(client->client_fd, "ko\n");
    remove_item_from_inventory(client->player, resource_type, 1);
    server->map[client->player->pos_y]
        [client->player->pos_x].resources[resource_type]++;
    server->current_resources[resource_type]++;
    send_pin_to_all(server, client);
    send_bct_to_all_graphical_clients(server, client->player->pos_x,
        client->player->pos_y);
    write_command_output(client->client_fd, "ok\n");
}
