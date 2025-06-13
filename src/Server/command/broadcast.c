/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** broadcast
*/
#include "../include/command.h"
#include "../include/graphical_commands.h"
#include "../include/server.h"
#include "../include/parsing.h"
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
    if (dx > 0 && dy < 0)
        return 2;
    if (dx == 0 && dy < 0)
        return 3;
    if (dx < 0 && dy < 0)
        return 4;
    if (dx < 0 && dy == 0)
        return 5;
    if (dx < 0 && dy > 0)
        return 6;
    if (dx == 0 && dy > 0)
        return 7;
    if (dx > 0 && dy > 0)
        return 8;
    return 8;
}

static int calculate_direction(client_t *receiver, int dx, int dy)
{
    int base_direction;

    if (dx == 0 && dy == 0)
        return 0;
    base_direction = get_base_direction(dx, dy);
    if (base_direction == 0)
        return 0;
    return ((base_direction - 1 + 2 * receiver->player->rotation) % 8) + 1;
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

static int calculate_message_length(char **buffer, int arr_length)
{
    int total_len = 0;

    for (int i = 1; i < arr_length; i++) {
        total_len += strlen(buffer[i]);
        if (i > 1)
            total_len += 1;
    }
    total_len += 1;
    return total_len;
}

static void concatenate_message_parts(char **buffer,
    int arr_length, char *message)
{
    int current_pos = 0;

    message[0] = '\0';
    for (int i = 1; i < arr_length; i++) {
        if (i > 1) {
            strcpy(message + current_pos, " ");
            current_pos += 1;
        }
        strcpy(message + current_pos, buffer[i]);
        current_pos += strlen(buffer[i]);
    }
}

static char *build_broadcast_message(char **buffer)
{
    int arr_length = arr_len(buffer);
    int total_len;
    char *message;

    if (!buffer || arr_length < 2)
        return NULL;
    total_len = calculate_message_length(buffer, arr_length);
    message = malloc(total_len);
    if (!message)
        return NULL;
    concatenate_message_parts(buffer, arr_length, message);
    return message;
}

void broadcast(server_t *server, client_t *user, char **buffer)
{
    char *message;
    client_t *current;

    if (!user || !user->player || !buffer || arr_len(buffer) < 2)
        return write_command_output(user->client_fd, "ko\n");
    message = build_broadcast_message(buffer);
    if (!message)
        return write_command_output(user->client_fd, "ko\n");
    current = server->client;
    if (current)
        current = current->next;
    command_pbc(server, user, message);
    while (current) {
        if (current->player && current != user && current->type != GRAPHICAL)
            send_broadcast_to_client(server, user, current, message);
        current = current->next;
    }
    free(message);
    write_command_output(user->client_fd, "ok\n");
}
