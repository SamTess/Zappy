/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** set_object
*/

#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <string.h>

void set_object(game_t *game, zappy_client_t *client,
    zappy_client_t *clients, char **buffer)
{
    resource_type_t resource_type;

    if (!game || !client || !client->client || !client->player ||
        !clients || arr_len(buffer) != 2)
        return write_command_output_buffer(client->client, "ko\n");
    resource_type = determine_type(buffer[1]);
    if (resource_type == COUNT)
        return write_command_output_buffer(client->client, "ko\n");
    if (how_many_in_inventory(client->player, resource_type) <= 0)
        return write_command_output_buffer(client->client, "ko\n");
    remove_item_from_inventory(client->player, resource_type, 1);
    game->map[client->player->pos_y]
        [client->player->pos_x].resources[resource_type]++;
    game->current_resources[resource_type]++;
    command_pdr(game, client, clients, resource_type);
    send_pin_to_all(game, clients, client);
    send_bct_to_all_graphical_clients(game, clients, client->player->pos_x,
        client->player->pos_y);
    write_command_output_buffer(client->client, "ok\n");
}
