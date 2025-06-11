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

static void send_bct_command(server_t *server, client_t *client, int x, int y)
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

void send_bct_to_all_graphical_clients(server_t *server, int x, int y)
{
    graphical_client_t *current = server->graphical_clients;

    if (!server || !server->graphical_clients)
        return;
    while (current) {
        send_bct_command(server, current->client, x, y);
        current = current->next;
    }
}

void send_mtc_to_all_graphical_clients(server_t *server)
{
    graphical_client_t *current = server->graphical_clients;

    if (!server || !server->graphical_clients)
        return;
    while (current) {
        send_tile_content_to_one_client(server, current->client);
        current = current->next;
    }
}

void send_tile_content_to_one_client(server_t *server, client_t *client)
{
    if (!server || !client || !server->graphical_clients)
        return;
    for (int y = 0; y < server->parsed_info->height; y++) {
        for (int x = 0; x < server->parsed_info->width; x++) {
            send_bct_command(server, client, x, y);
        }
    }
}

static bool check_if_length_is_valid_bct(const char *buffer, int x, int y)
{
    size_t expected_length = 0;

    expected_length = snprintf(NULL, 0, "bct %d %d\n", x, y);
    if (expected_length != strlen(buffer))
        return false;
    return true;
}

void command_bct(server_t *server, client_t *client, char *buffer)
{
    int x = 0;
    int y = 0;

    if (!server || !client || !buffer || !server->graphical_clients ||
        strlen(buffer) < 7 ||
        sscanf(buffer, "bct %d %d\n", &x, &y) != 2 ||
        !check_if_length_is_valid_bct(buffer, x, y) ||
        x < 0 || y < 0 ||
        y >= server->parsed_info->height ||
        x >= server->parsed_info->width)
        return write_command_output(client->client_fd, "sbp\n");
    send_bct_command(server, client, x, y);
}

void command_mtc(server_t *server, client_t *client, char *buffer)
{
    (void)buffer;
    if (!server || !client || !server->graphical_clients)
        return;
    if (client->type != GRAPHICAL)
        return write_command_output(client->client_fd, "ko\n");
    send_tile_content_to_one_client(server, client);
}
