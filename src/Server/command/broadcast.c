/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** broadcast
*/
#include "../include/command.h"
#include "../include/zappy.h"
#include "../include/parsing.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int adapt_rota_player(enum rotation_e rota_player)
{
    switch (rota_player) {
        case UP:
            return 0;
        case LEFT:
            return 1;
        case DOWN:
            return 2;
        case RIGHT:
            return 3;
        default:
            return -1;
    }
}

static int get_tile_for_north_orientation(int source_rel_x, int source_rel_y)
{
    if (source_rel_x == 0 && source_rel_y < 0)
        return 1;
    if (source_rel_x < 0 && source_rel_y < 0)
        return 2;
    if (source_rel_x < 0 && source_rel_y == 0)
        return 3;
    if (source_rel_x < 0 && source_rel_y > 0)
        return 4;
    if (source_rel_x == 0 && source_rel_y > 0)
        return 5;
    if (source_rel_x > 0 && source_rel_y > 0)
        return 6;
    if (source_rel_x > 0 && source_rel_y == 0)
        return 7;
    if (source_rel_x > 0 && source_rel_y < 0)
        return 8;
    return 0;
}

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

static int calculate_direction(player_t *receiver, int source_rel_x,
    int source_rel_y)
{
    int base_tile_for_north;
    int new_rota = adapt_rota_player(receiver->rotation);

    if (source_rel_x == 0 && source_rel_y == 0)
        return 0;
    base_tile_for_north = get_tile_for_north_orientation(source_rel_x,
        source_rel_y);
    if (base_tile_for_north == 0)
        return 0;
    return ((base_tile_for_north - 1 + (2 * new_rota)
        + 8) % 8) + 1;
}

static int send_broadcast_to_client(game_t *game, zappy_client_t *sender,
    zappy_client_t *receiver, char *message)
{
    int direction = 0;
    size_t res_size = snprintf(NULL, 0, "message %d, %s\n", 0, message) + 1;
    char *response = calloc(res_size, sizeof(char));
    int source_rel_x;
    int source_rel_y;

    if (!receiver || !receiver->player || receiver == sender)
        return 84;
    if (response == NULL)
        return 84;
    source_rel_x = calculate_shortest_distance_x(receiver->player->pos_x,
        sender->player->pos_x, game->parsed_info->width);
    source_rel_y = calculate_shortest_distance_y(receiver->player->pos_y,
        sender->player->pos_y, game->parsed_info->height);
    direction = calculate_direction(receiver->player,
        source_rel_x, source_rel_y);
    snprintf(response, res_size, "message %d, %s\n", direction, message);
    write_command_output_buffer(receiver->client, response);
    free(response);
    return 0;
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

void broadcast(game_t *game, zappy_client_t *sender,
    zappy_client_t *clients, char **buffer)
{
    char *message;
    zappy_client_t *current;

    if (!sender || !sender->player || !buffer || arr_len(buffer) < 2)
        return write_command_output_buffer(sender->client, "ko\n");
    message = build_broadcast_message(buffer);
    if (!message)
        return write_command_output_buffer(sender->client, "ko\n");
    current = clients;
    command_pbc(game, sender, clients, message);
    for (int temp = 0; current != NULL; current = current->next) {
        if (current->player && current != sender && current->type != GRAPHICAL)
            temp = send_broadcast_to_client(game, sender, current, message);
        if (temp == 84)
            return write_command_output_buffer(sender->client, "ko\n");
    }
    free(message);
    write_command_output_buffer(sender->client, "ok\n");
}
