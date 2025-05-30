/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** left
*/
#include "../include/command.h"
#include <stdio.h>

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
    if (client->player->rotation == UP)
        client->player->rotation = LEFT;
    if (client->player->rotation == DOWN)
        client->player->rotation = RIGHT;
    if (client->player->rotation == LEFT)
        client->player->rotation = DOWN;
    if (client->player->rotation == RIGHT)
        client->player->rotation = UP;
    write_command_output(client->client_fd, "ok\n");
}
