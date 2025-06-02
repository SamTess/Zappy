/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** forward
*/
#include "../include/command.h"
#include <stdio.h>
#include <unistd.h>

static void wrap_position(server_t *server, client_t *client)
{
    if (client->player->pos_y >= server->parsed_info->height)
        client->player->pos_y = 0;
    if (client->player->pos_y < 0)
        client->player->pos_y = server->parsed_info->height - 1;
    if (client->player->pos_x < 0)
        client->player->pos_x = server->parsed_info->width - 1;
    if (client->player->pos_x >= server->parsed_info->width)
        client->player->pos_x = 0;
}

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
    wrap_position(server, client);
    write_command_output(client->client_fd, "ok\n");
}
