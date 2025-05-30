/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** right
*/
#include "../include/command.h"
#include <stdio.h>

void right(server_t *server, client_t *client, char *buffer)
{
    (void)server;
    (void)buffer;
    if (client->player->rotation != UP && client->player->rotation != DOWN
        && client->player->rotation != LEFT
        && client->player->rotation != RIGHT){
        perror("Unexpected right rotation");
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    if (client->player->rotation == UP)
        client->player->rotation = RIGHT;
    if (client->player->rotation == DOWN)
        client->player->rotation = LEFT;
    if (client->player->rotation == LEFT)
        client->player->rotation = UP;
    if (client->player->rotation == RIGHT)
        client->player->rotation = DOWN;
    write_command_output(client->client_fd, "ok\n");
}
