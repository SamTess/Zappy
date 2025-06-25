/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** inventory
*/

#include "../include/command.h"
#include "../include/player.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

void inventory(server_t *server, client_t *client, char **buffer)
{
    char *content;

    if (!server || !client || !client->player || arr_len(buffer) != 1)
        return write_command_output_buffer(client, "ko\n");
    content = get_inventory_content(client->player);
    write_command_output_buffer(client, content);
    free(content);
}
