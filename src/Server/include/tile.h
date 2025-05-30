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

typedef enum ResourceType {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    COUNT
} ResourceType_t;

typedef struct tile_s {
    int *player_ids;
    int player_count;
    int player_capacity;
    int resources[COUNT];
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
void distribute_resources(tile_t **map, int width, int height);

#endif /* !TILE_H_ */
