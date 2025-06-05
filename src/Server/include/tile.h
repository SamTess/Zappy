/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tile
*/

#ifndef TILE_H_
    #define TILE_H_
    #define FOOD_D 0.5
    #define LINEMATE_D 0.3
    #define DERAUMERE_D 0.15
    #define SIBUR_D 0.1
    #define MENDIANE_D 0.1
    #define PHIRAS_D 0.08
    #define THYSTAME_D 0.05

typedef struct server_s server_t;
typedef enum resource_type {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    COUNT
} resource_type_t;

typedef struct tile_s {
    int *player_ids;
    int player_count;
    int player_capacity;
    int resources[COUNT];
    int *egg_ids;
    int egg_count;
    int egg_capacity;
} tile_t;

typedef struct resource_dist_s {
    tile_t **map;
    int width;
    int height;
    int *tile_indices;
} resource_dist_t;

void tile_init(tile_t *tile);
void tile_free(tile_t *tile);
int tile_add_player(tile_t *tile, int player_id);
int tile_remove_player(tile_t *tile, int player_id);
void add_egg_to_tile(tile_t *tile, int egg_id);
void remove_egg_from_tile(tile_t *tile, int egg_id);
void distribute_resources(tile_t **map, server_t *server,
    int *total_resources, int *current_resources);
void respawn_resources(tile_t **map, server_t *server,
    int *total_resources, int *current_resources);

#endif /* !TILE_H_ */
