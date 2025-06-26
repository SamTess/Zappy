/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** fork
*/
#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include "../include/egg.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>

static int get_next_egg_id(game_t *game)
{
    egg_t *current = game->eggs;
    int max_id = 0;

    while (current) {
        if (current->egg_id > max_id)
            max_id = current->egg_id;
        current = current->next;
    }
    return max_id + 1;
}

void fork_c(game_t *game, zappy_client_t *client, zappy_client_t *clients, char **buffer)
{
    egg_t *new_egg;
    int egg_id;

    if (!game || !client || !client->client || !client->player ||
        !clients || arr_len(buffer) != 1) {
        write_command_output_buffer(client->client, "ko\n");
        return;
    }
    egg_id = get_next_egg_id(game);
    new_egg = create_egg(egg_id, client->player->pos_x, client->player->pos_y,
        client->player->team_name);
    if (!new_egg) {
        write_command_output_buffer(client->client, "ko\n");
        return;
    }
    add_egg(game, new_egg);
    send_enw_command(game, clients, client, egg_id);
    write_command_output_buffer(client->client, "ok\n");
}
