/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg
*/

#include "../include/tile.h"
#include "../include/server.h"
#include "../include/egg.h"
#include <stdlib.h>
#include <string.h>

egg_t *create_egg(int egg_id, int pos_x, int pos_y, char *team)
{
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (new_egg == NULL)
        return NULL;
    new_egg->egg_id = egg_id;
    new_egg->pos_x = pos_x;
    new_egg->pos_y = pos_y;
    new_egg->next = NULL;
    new_egg->team_name = strdup(team);
    if (new_egg->team_name == NULL)
        server_err("Egg team allocation failed\n");
    return new_egg;
}

static int remove_id_from_array(int **array, int *count, int id)
{
    int last_idx = 0;

    if (array == NULL || *array == NULL || count == NULL)
        return -1;
    for (int i = 0; i < *count; i++) {
        if ((*array)[i] == id) {
            last_idx = *count - 1;
            (*array)[i] = (*array)[last_idx];
            *count -= 1;
            return 0;
        }
    }
    return -1;
}

static void remove_node(egg_t *current, egg_t *prev, server_t *server)
{
    egg_t *node_to_delete = NULL;

    node_to_delete = current;
    if (prev == NULL)
        server->eggs = current->next;
    else
        prev->next = current->next;
    if (node_to_delete->team_name)
        free(node_to_delete->team_name);
    free(node_to_delete);
    return;
}

static void remove_egg_by_id(server_t *server, int id)
{
    egg_t *current = server->eggs;
    egg_t *prev = NULL;

    while (current != NULL) {
        if (current->egg_id == id) {
            remove_node(current, prev, server);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void remove_egg_from_tile(tile_t *tile, int egg_id)
{
    if (tile == NULL)
        return;
    remove_id_from_array(&tile->egg_ids, &tile->egg_count, egg_id);
}

void remove_egg(server_t *server, int id, tile_t *tile)
{
    if (tile == NULL)
        return;
    remove_egg_from_tile(tile, id);
    remove_egg_by_id(server, id);
}

void add_egg_to_tile(tile_t *tile, int egg_id)
{
    int new_capacity;
    int *new_egg_ids;

    if (tile == NULL)
        return;
    if (tile->egg_count >= tile->egg_capacity) {
        if (tile->egg_capacity == 0)
            new_capacity = 4;
        else
            new_capacity = tile->egg_capacity * 2;
        new_egg_ids = realloc(tile->egg_ids, sizeof(int) * new_capacity);
        if (new_egg_ids == NULL)
            return;
        tile->egg_ids = new_egg_ids;
        tile->egg_capacity = new_capacity;
    }
    tile->egg_ids[tile->egg_count] = egg_id;
    tile->egg_count++;
}

void add_egg(server_t *server, egg_t *egg)
{
    tile_t *tile = NULL;

    if (server == NULL || egg == NULL)
        return;
    egg->next = server->eggs;
    server->eggs = egg;
    tile = &server->map[egg->pos_y][egg->pos_x];
    add_egg_to_tile(tile, egg->egg_id);
}
