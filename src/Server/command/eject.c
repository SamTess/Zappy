/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** eject
*/

#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void convert_rotation_to_vector(client_t *client, float *x, float *y)
{
    switch (client->player->rotation) {
        case RIGHT:
            *x = 1.0;
            *y = 0.0;
            break;
        case DOWN:
            *x = 0.0;
            *y = 1.0;
            break;
        case LEFT:
            *x = -1.0;
            *y = 0.0;
            break;
        case UP:
        default:
            *x = 0.0;
            *y = -1.0;
            break;
    }
}

static void wrap_position(server_t *server, int *x, int *y)
{
    if (*y >= server->parsed_info->height)
        *y = 0;
    if (*y < 0)
        *y = server->parsed_info->height - 1;
    if (*x < 0)
        *x = server->parsed_info->width - 1;
    if (*x >= server->parsed_info->width)
        *x = 0;
}

static int direction_push(float x, float y)
{
    if (x == 1 && y == 0)
        return 1;
    if (x == 0 && y == 1)
        return 7;
    if (x == 0 && y == -1)
        return 3;
    if (x == -1 && y == 0)
        return 5;
    return -1;
}

static char *get_string_to_send(float x, float y)
{
    char *string_to_send = malloc(sizeof(char) * 10);

    if (!string_to_send)
        server_err("Malloc failed in get_string_to_send");
    sprintf(string_to_send, "eject: %d\n", direction_push(x, y));
    return string_to_send;
}

static void push_single_client(server_t *server, client_t *client,
    client_t *tmp, char *msg)
{
    int new_x = client->player->pos_x +
        (int)(tmp->player->pos_x == client->player->pos_x);
    int new_y = client->player->pos_y +
        (int)(tmp->player->pos_y == client->player->pos_y);

    wrap_position(server, &new_x, &new_y);
    tmp->player->pos_x = new_x;
    tmp->player->pos_y = new_y;
    tile_add_player(&server->map[new_y][new_x], tmp->client_id);
    send_ppo_command(server, tmp->client_id);
    write_command_output(tmp->client_fd, msg);
}

void push_client(server_t *server, client_t *client, float x, float y)
{
    client_t *tmp = server->client->next;
    int old_x = client->player->pos_x;
    int old_y = client->player->pos_y;
    char *msg = get_string_to_send(x, y);

    while (tmp) {
        if (tmp->client_id == client->client_id || tmp->type == GRAPHICAL) {
            tmp = tmp->next;
            continue;
        }
        if (tmp->player->pos_x == old_x && tmp->player->pos_y == old_y)
            push_single_client(server, client, tmp, msg);
        tmp = tmp->next;
    }
    free(msg);
}

static void push_eggs(server_t *server, int old_x, int old_y)
{
    egg_t *tmp_egg = server->eggs;
    egg_t *next_egg;

    tmp_egg = server->eggs;
    while (tmp_egg) {
        next_egg = tmp_egg->next;
        if (tmp_egg->pos_x == old_x && tmp_egg->pos_y == old_y) {
            send_edi_command(server, tmp_egg->egg_id);
            remove_egg(server, tmp_egg->egg_id, &server->map[old_y][old_x]);
        }
        tmp_egg = next_egg;
    }
}

void eject(server_t *server, client_t *client, char **buffer)
{
    float x = 0;
    float y = 0;

    if (!client || !client->player || arr_len(buffer) != 1)
        return write_command_output(client->client_fd, "ko\n");
    convert_rotation_to_vector(client, &x, &y);
    push_client(server, client, x, y);
    push_eggs(server, client->player->pos_x, client->player->pos_y);
    command_pex(server, client);
    write_command_output(client->client_fd, "ok\n");
}
