/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** broadcast
*/
#include "../include/command.h"
#include "../include/server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int calculate_shortest_distance_x(int sender_x,
    int receiver_x, int width)
{
    int direct_distance = receiver_x - sender_x;
    int wrap_distance_right = (receiver_x + width) - sender_x;
    int wrap_distance_left = receiver_x - (sender_x + width);

    if (abs(direct_distance) <= abs(wrap_distance_right) &&
        abs(direct_distance) <= abs(wrap_distance_left))
        return direct_distance;
    if (abs(wrap_distance_right) <= abs(wrap_distance_left))
        return wrap_distance_right;
    return wrap_distance_left;
}

static int calculate_shortest_distance_y(int sender_y,
    int receiver_y, int height)
{
    int direct_distance = receiver_y - sender_y;
    int wrap_distance_down = (receiver_y + height) - sender_y;
    int wrap_distance_up = receiver_y - (sender_y + height);

    if (abs(direct_distance) <= abs(wrap_distance_down) &&
        abs(direct_distance) <= abs(wrap_distance_up))
        return direct_distance;
    if (abs(wrap_distance_down) <= abs(wrap_distance_up))
        return wrap_distance_down;
    return wrap_distance_up;
}

static int get_base_direction(int dx, int dy)
{
    if (dx == 0 && dy == 0)
        return 0;
    if (dx > 0 && dy == 0)
        return 1;
    if (dx > 0 && dy > 0)
        return 2;
    if (dx == 0 && dy > 0)
        return 3;
    if (dx < 0 && dy > 0)
        return 4;
    if (dx < 0 && dy == 0)
        return 5;
    if (dx < 0 && dy < 0)
        return 6;
    if (dx == 0 && dy < 0)
        return 7;
    return 8;
}

static int adjust_direction_for_rotation(int direction, int rotation)
{
    if (rotation == UP)
        return direction;
    if (rotation == RIGHT)
        return ((direction - 2 - 1 + 8) % 8) + 1;
    if (rotation == DOWN)
        return ((direction - 4 - 1 + 8) % 8) + 1;
    return ((direction + 2 - 1) % 8) + 1;
}

static int calculate_direction(client_t *receiver, int dx, int dy)
{
    int base_direction = get_base_direction(dx, dy);

    if (base_direction == 0)
        return 0;
    return adjust_direction_for_rotation(base_direction,
        receiver->player->rotation);
}

static void send_broadcast_to_client(server_t *server, client_t *sender,
    client_t *receiver, char *message)
{
    int dx = 0;
    int dy = 0;
    int direction = 0;
    size_t res_size = snprintf(NULL, 0, "message %d, %s\n", 0, message) + 1;
    char *response = calloc(res_size, sizeof(char));

    if (!receiver || !receiver->player || receiver == sender)
        return;
    if (response == NULL)
        server_err("Malloc failed for allocating response for broadcast");
    dx = calculate_shortest_distance_x(sender->player->pos_x,
        receiver->player->pos_x, server->parsed_info->width);
    dy = calculate_shortest_distance_y(sender->player->pos_y,
        receiver->player->pos_y, server->parsed_info->height);
    direction = calculate_direction(receiver, dx, dy);
    snprintf(response, res_size, "message %d, %s\n", direction, message);
    write_command_output(receiver->client_fd, response);
    free(response);
}

void broadcast(server_t *server, client_t *user, char *buffer)
{
    char *message;
    client_t *current;

    if (strlen(buffer) <= 11 || buffer[strlen(buffer) - 1] != '\n') {
        write_command_output(user->client_fd, "ko\n");
        return;
    }
    message = buffer + 10;
    if (message[strlen(message) - 1] == '\n')
        message[strlen(message) - 1] = '\0';
    current = server->client;
    if (current)
        current = current->next;
    while (current) {
        if (current->player && current != user)
            send_broadcast_to_client(server, user, current, message);
        current = current->next;
    }
    write_command_output(user->client_fd, "ok\n");
}
