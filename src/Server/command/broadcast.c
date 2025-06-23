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
    int dx = sender_x - receiver_x;

    if (dx > width / 2)
        dx -= width;
    if (dx < (-width) / 2)
        dx += width;
    return dx;
}

static int calculate_shortest_distance_y(int sender_y,
    int receiver_y, int height)
{
    int dy = sender_y - receiver_y;

    if (dy > height / 2)
        dy -= height;
    if (dy < (-height) / 2)
        dy += height;
    return dy;
}

static int get_tile_for_north_bis(double angle)
{
    if (angle < 0)
        angle += 360.0;
    if (angle >= 337.5 || angle < 22.5)
        return 1;
    if (angle >= 22.5 && angle < 67.5)
        return 8;
    if (angle >= 67.5 && angle < 112.5)
        return 7;
    if (angle >= 112.5 && angle < 157.5)
        return 6;
    if (angle >= 157.5 && angle < 202.5)
        return 5;
    if (angle >= 202.5 && angle < 247.5)
        return 4;
    if (angle >= 247.5 && angle < 292.5)
        return 3;
    if (angle >= 292.5 && angle < 337.5)
        return 2;
    return 1;
}

static int get_tile_for_north_orientation(int dx, int dy)
{
    if (dx == 0 && dy == 0)
        return 0;
    return get_tile_for_north_bis(atan2(-dy, dx) *
        180.0 / 3.141592653589793238460);
}

static int calculate_direction(client_t *receiver, int dx, int dy)
{
    int base_tile_for_north = get_tile_for_north_orientation(dx, dy);
    int orientation_offset = (receiver->player->rotation - 1) * 2;

    if (dx == 0 && dy == 0)
        return 0;
    return ((base_tile_for_north - 1 - orientation_offset + 8) % 8) + 1;
}

static void send_broadcast_to_client(server_t *server, client_t *sender,
    client_t *receiver, char *message)
{
    int dx = calculate_shortest_distance_x(sender->player->pos_x,
        receiver->player->pos_x, server->parsed_info->width);
    int dy = calculate_shortest_distance_y(sender->player->pos_y,
        receiver->player->pos_y, server->parsed_info->height);
    int direction = calculate_direction(receiver, dx, dy);
    size_t res_size = snprintf(NULL, 0, "message %d, %s\n", 0, message) + 1;
    char *response = calloc(res_size, sizeof(char));

    if (!response)
        server_err("Calloc for response failed in broadcast");
    if (!receiver || !receiver->player || receiver == sender)
        return;
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
