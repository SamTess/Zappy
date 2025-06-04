/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** scheduled_command_utils
*/
#include "../include/scheduled_command.h"
#include "../include/command.h"
#include <stdlib.h>

void execute_scheduled_command(server_t *server, client_t *client)
{
    scheduled_command_t *cmd;

    if (!client || !client->player || !client->player->pending_command)
        return;
    cmd = client->player->pending_command;
    if (cmd->func)
        cmd->func(server, client, cmd->args);
    cmd->args = NULL;
    cmd->execute_at_tick = 0;
    cmd->func = NULL;
}
