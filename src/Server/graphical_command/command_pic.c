/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pic
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

static char *get_pic_buffer(int x, int y, int level, tile_t *tile)
{
    int size = 0;
    char *buffer = NULL;
    int offset = 0;

    size = snprintf(NULL, 0, "pic %d %d %d", x, y, level);
    for (int i = 0; i < tile->player_count; i++)
        size += snprintf(NULL, 0, " #%d", tile->player_ids[i]);
    size += 2;
    buffer = malloc(size);
    if (!buffer)
        return NULL;
    offset = snprintf(buffer, size, "pic %d %d %d", x, y, level);
    for (int i = 0; i < tile->player_count; i++)
        offset += snprintf(buffer + offset, size - offset, " #%d",
            tile->player_ids[i]);
    snprintf(buffer + offset, size - offset, "\n");
    return buffer;
}

void command_pic(server_t *server, int x, int y, int level)
{
    graphical_client_t *current = NULL;
    char *buffer = NULL;
    tile_t *tile = NULL;

    if (!server || !server->graphical_clients)
        return;
    if (y < 0 || x < 0 || y >= server->parsed_info->height
        || x >= server->parsed_info->width)
        return;
    tile = &server->map[y][x];
    buffer = get_pic_buffer(x, y, level, tile);
    if (!buffer)
        return;
    current = server->graphical_clients;
    while (current) {
        write_command_output(current->client->client_fd, buffer);
        current = current->next;
    }
    free(buffer);
}
