/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** right
*/
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/parsing.h"
#include <stdio.h>

static void change_rot(client_t *client)
{
    switch (client->player->rotation) {
    case UP:
        client->player->rotation = RIGHT;
        break;
    case DOWN:
        client->player->rotation = LEFT;
        break;
    case LEFT:
        client->player->rotation = UP;
        break;
    case RIGHT:
        client->player->rotation = DOWN;
        break;
    default:
        break;
    }
}

void right(server_t *server, client_t *client, char **buffer)
{

    if (!server || !client || !client->player || arr_len(buffer) != 1)
        return write_command_output(client->client_fd, "ko\n");
    if (client->player->rotation != UP && client->player->rotation != DOWN
        && client->player->rotation != LEFT
        && client->player->rotation != RIGHT) {
        perror("Unexpected right rotation");
        return write_command_output(client->client_fd, "ko\n");
    }
    change_rot(client);
    send_ppo_command(server, client->client_id);
    write_command_output(client->client_fd, "ok\n");
}
