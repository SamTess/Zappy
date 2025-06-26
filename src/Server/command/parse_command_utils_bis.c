/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** parse_command_utils_bis
*/
#include "../include/command.h"
#include "../include/zappy.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int check_disconnect(int bytes_read, server_t *server, zappy_client_t **clients, zappy_client_t *user)
{
    if (bytes_read <= 0){
        command_pdi(NULL, user, *clients);
        remove_fd(server, clients, user->client->client_fd);
        return 1;
    }
    return 0;
}

bool handle_socket_read(server_t *server, zappy_client_t **clients, zappy_client_t *user)
{
    char temp_read_storage[1024];
    int bytes_read;

    bytes_read = read(user->client->client_fd,
        temp_read_storage, sizeof(temp_read_storage) - 1);
    if (check_disconnect(bytes_read, server, clients, user) == 1)
        return false;
    for (int i = 0; i < bytes_read; i++) {
        if (add_to_circular_buffer(&user->client->read_buffer,
            temp_read_storage[i]) == -1)
            break;
    }
    return true;
}

static void add_type_graphic_user(zappy_client_t *user)
{
    user->type = GRAPHICAL;
    free(user->player->command_queue);
    free(user->player->pending_cmd);
    free(user->player);
    user->player = NULL;
}

static bool is_valid_team_name(char *team_name, game_t *game,
    zappy_client_t *user, zappy_client_t *clients)
{
    if (!team_name || !game ||
        !game->parsed_info || !game->parsed_info->names)
        return false;
    if (strlen(team_name) < 2 || team_name[strlen(team_name) - 1] != '\n')
        return false;
    team_name[strlen(team_name) - 1] = '\0';
    if (strcmp(team_name, "GRAPHIC") == 0) {
        add_type_graphic_user(user);
        return true;
    }
    for (int i = 0; game->parsed_info->names[i] != NULL; i++) {
        if (strcmp(team_name, game->parsed_info->names[i]) == 0){
            user->player->team_name = strdup(team_name);
            user->type = AI;
            init_new_player_pos(game, user, clients);
            return true;
        }
    }
    return false;
}

int add_string_to_write_buffer(circular_buffer_t *cb, char *msg)
{
    size_t len = strlen(msg);

    for (size_t i = 0; i < len; i++) {
        if (add_to_circular_buffer(cb, msg[i]) == -1)
            return -1;
    }
    return 0;
}

void flush_client_write_buffer(client_t *client)
{
    int available = client->write_buffer.count;
    char *temp;

    if (!client || client->write_buffer.count == 0)
        return;
    temp = malloc(sizeof(char) * (available + 1));
    if (!temp)
        return;
    for (int i = 0; i < available; i++) {
        temp[i] = client->write_buffer.buffer[client->write_buffer.start];
        client->write_buffer.start =
            (client->write_buffer.start + 1) % BUFFER_SIZE;
        client->write_buffer.count--;
    }
    temp[available] = '\0';
    write_command_output(client->client_fd, temp);
    client->need_write = false;
    free(temp);
}

bool can_connect(game_t *game, zappy_client_t *user, char *buffer, zappy_client_t *clients)
{
    if (!is_valid_team_name(buffer, game, user, clients)){
        write_command_output_buffer(user->client, "ko\n");
        return false;
    }
    if (user->type != GRAPHICAL &&
        connect_nbr_srv(game, user->player->team_name) < 0) {
        if (user->player->team_name) {
            free(user->player->team_name);
            user->player->team_name = NULL;
        }
        write_command_output_buffer(user->client, "ko\n");
        return false;
    }
    return true;
}
