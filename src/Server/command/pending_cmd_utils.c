/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pending_cmd_utils
*/
#include "../include/pending_cmd.h"
#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/game.h"
#include <stdlib.h>

void execute_pending_cmd(game_t *game, zappy_client_t *client,
    zappy_client_t *clients)
{
    pending_cmd_t *cmd;
    char **args_array;

    if (!game || !client || !client->player || !client->client ||
        !client->player->pending_cmd)
        return;
    cmd = client->player->pending_cmd;
    if (cmd->func && cmd->args) {
        args_array = str_to_word_arr(cmd->args, " ");
        cmd->func(game, client, clients, args_array);
        free_arr(args_array);
    }
    if (cmd->args)
        free(cmd->args);
    cmd->args = NULL;
    cmd->func = NULL;
}
