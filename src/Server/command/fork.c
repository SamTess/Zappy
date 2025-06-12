/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** fork
*/
#include "../include/command.h"
#include "../include/server.h"
#include "../include/egg.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

static int get_next_egg_id(server_t *server)
{
    egg_t *current = server->eggs;
    int max_id = 0;

    while (current) {
        if (current->egg_id > max_id)
            max_id = current->egg_id;
        current = current->next;
    }
    return max_id + 1;
}

void fork_c(server_t *server, client_t *client, char **buffer)
{
    egg_t *new_egg;
    int egg_id;

    if (!client || !client->player || arr_len(buffer) != 1) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    egg_id = get_next_egg_id(server);
    new_egg = create_egg(egg_id, client->player->pos_x, client->player->pos_y,
        client->player->team_name);
    if (!new_egg) {
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    add_egg(server, new_egg);
    send_enw_command(server, client, egg_id);
    write_command_output(client->client_fd, "ok\n");
}
