/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_bct
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int calculate_size_bct_command(int x, int y, tile_t *tile)
{
    return snprintf(NULL, 0, "bct %d %d %d %d %d %d %d %d %d\n",
            x, y,
            tile->resources[FOOD],
            tile->resources[LINEMATE],
            tile->resources[DERAUMERE],
            tile->resources[SIBUR],
            tile->resources[MENDIANE],
            tile->resources[PHIRAS],
            tile->resources[THYSTAME]);
}

static char *get_buffer_bct_command(int x, int y, tile_t *tile)
{
    int size = calculate_size_bct_command(x, y, tile);
    char *buffer = malloc(size + 1);

    if (!buffer)
        return NULL;
    snprintf(buffer, size + 1, "bct %d %d %d %d %d %d %d %d %d\n",
            x, y,
            tile->resources[FOOD],
            tile->resources[LINEMATE],
            tile->resources[DERAUMERE],
            tile->resources[SIBUR],
            tile->resources[MENDIANE],
            tile->resources[PHIRAS],
            tile->resources[THYSTAME]);
    return buffer;
}

void send_bct_command(server_t *server, client_t *client, int x, int y)
{
    tile_t *tile = NULL;
    char *buffer = NULL;

    if (!server || !client ||
        x < 0 || y < 0 ||
        y >= server->parsed_info->height || x >= server->parsed_info->width)
        return;
    tile = &server->map[y][x];
    buffer = get_buffer_bct_command(x, y, tile);
    write_command_output(client->client_fd, buffer);
    free(buffer);
}

void send_tile_content_to_one_client(server_t *server, client_t *client)
{
    if (!server || !client)
        return;
    for (int y = 0; y < server->parsed_info->height; y++) {
        for (int x = 0; x < server->parsed_info->width; x++) {
            send_bct_command(server, client, x, y);
        }
    }
}

void command_bct(server_t *server, client_t *client, char *buffer)
{
    int x = 0;
    int y = 0;

    if (!server || !client || !buffer)
        return;
    if (client->type != GRAPHICAL)
        return write_command_output(client->client_fd, "ko\n");
    if (strlen(buffer) < 7)
        return write_command_output(client->client_fd, "ko\n");
    if (sscanf(buffer, "bct %d %d\n", &x, &y) != 2)
        return write_command_output(client->client_fd, "ko\n");
    if (x < 0 || y < 0 || y >= server->parsed_info->height ||
        x >= server->parsed_info->width)
        return write_command_output(client->client_fd, "ko\n");
    send_bct_command(server, client, x, y);
}

void command_mtc(server_t *server, client_t *client, char *buffer)
{
    (void)buffer;
    if (!server || !client)
        return;
    if (client->type != GRAPHICAL)
        return write_command_output(client->client_fd, "ko\n");
    send_tile_content_to_one_client(server, client);
}
