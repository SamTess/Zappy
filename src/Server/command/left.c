/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** left
*/
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include <stdio.h>

static void change_rot(client_t *client)
{
    switch (client->player->rotation) {
    case UP:
        client->player->rotation = LEFT;
        break;
    case DOWN:
        client->player->rotation = RIGHT;
        break;
    case LEFT:
        client->player->rotation = DOWN;
        break;
    case RIGHT:
        client->player->rotation = UP;
        break;
    default:
        break;
    }
}

void left(server_t *server, client_t *client, char *buffer)
{
    (void)server;
    (void)buffer;
    if (client->player->rotation != UP && client->player->rotation != DOWN
        && client->player->rotation != LEFT
        && client->player->rotation != RIGHT){
        perror("Unexpected left rotation");
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    change_rot(client);
    send_ppo_command(server, client->client_id);
    write_command_output(client->client_fd, "ok\n");
}
