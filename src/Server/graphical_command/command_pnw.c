/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** command_pnw
*/

#include "../include/server.h"
#include "../include/client.h"
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>
#include <stdlib.h>

static int calcul_size_pnw_command(client_t *client)
{
    if (!client || !client->player)
        return 0;
    return snprintf(NULL, 0, "pnw #%d %d %d %d %d %s\n",
                client->client_id,
                client->player->pos_x,
                client->player->pos_y,
                client->player->rotation,
                client->player->level,
                client->team);
}

void send_pnw_command(server_t *server, client_t *client, client_t *recipient)
{
    int size = 0;
    char *buffer = NULL;

    if (!server || !client || !client->player)
        return;
    size = calcul_size_pnw_command(client);
    buffer = malloc(size + 1);
    if (!buffer)
        return;
    snprintf(buffer, size + 1, "pnw #%d %d %d %d %d %s\n",
            client->client_id,
            client->player->pos_x,
            client->player->pos_y,
            client->player->rotation,
            client->player->level,
            client->player->team_name);
    write_command_output(recipient->client_fd, buffer);
    free(buffer);
}

void send_all_player_info_to_one_client(server_t *server, client_t *client)
{
    client_t *current = NULL;

    if (!server)
        return;
    current = server->client;
    while (current) {
        if (current->player && current->type == AI &&
            current->is_fully_connected) {
            send_pnw_command(server, current, client);
            send_pin_command(server, current, client);
            send_plv_command(server, current, client);
        }
        current = current->next;
    }
}
