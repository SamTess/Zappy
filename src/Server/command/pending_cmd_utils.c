/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pending_cmd_utils
*/
#include "../include/pending_cmd.h"
#include "../include/command.h"
#include <stdlib.h>

void execute_pending_cmd(server_t *server, client_t *client)
{
    pending_cmd_t *cmd;

    if (!client || !client->player || !client->player->pending_cmd)
        return;
    cmd = client->player->pending_cmd;
    if (cmd->func)
        cmd->func(server, client, cmd->args);
    cmd->args = NULL;
    cmd->func = NULL;
}
