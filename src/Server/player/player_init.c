/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player_init
*/

#include "../include/player.h"
#include "../include/server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void init_player(player_t *player, char *playerTeam)
{
    player->pos_x = 0;
    player->pos_y = 0;
    player->level = 1;
    player->life = 126;
    if (playerTeam != NULL) {
        player->team_name = strdup(playerTeam);
        if (player->team_name == NULL)
            malloc_failed(4);
    } else {
        player->team_name = NULL;
    }
    init_inventory(player);
}
