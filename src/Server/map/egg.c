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

egg_t *create_egg(int egg_id, int creator_id, int pos_x, int pos_y)
{
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (new_egg == NULL)
        return NULL;
    new_egg->egg_id = egg_id;
    new_egg->creator_id = creator_id;
    new_egg->pos_x = pos_x;
    new_egg->pos_y = pos_y;
    new_egg->next = NULL;
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

static void execute_remove_egg(egg_t *current, egg_t *prev,
    int id, server_t *server)
{
    if (current->egg_id == id) {
        if (prev == NULL) {
            server->eggs = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
        return;
    }
}

static void remove_egg_by_id(server_t *server, int id)
{
    egg_t *current = server->eggs;
    egg_t *prev = NULL;

    while (current != NULL) {
        execute_remove_egg(current, prev, id, server);
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
