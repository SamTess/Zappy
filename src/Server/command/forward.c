/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** forward
*/
#include "../include/command.h"
#include <stdio.h>
#include <unistd.h>

void forward(server_t *server, client_t *client, char *buffer)
{
    (void)server;
    (void)buffer;
    if (client->player->rotation != UP && client->player->rotation != DOWN
        && client->player->rotation != LEFT
        && client->player->rotation != RIGHT){
        perror("Unexpected forward rotation");
        write_command_output(client->client_fd, "ko\n");
        return;
    }
    if (client->player->rotation == UP)
        client->player->pos_y++;
    if (client->player->rotation == DOWN)
        client->player->pos_y--;
    if (client->player->rotation == LEFT)
        client->player->pos_x--;
    if (client->player->rotation == RIGHT)
        client->player->pos_x++;
    write_command_output(client->client_fd, "ok\n");
}
