/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#ifndef PLAYER_H_
    #define PLAYER_H_
    #include <stdbool.h>
    #include "tile.h"
    #include "pending_cmd.h"

enum rotation_e {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct player_inventory_s {
    resource_type_t type;
    char *name;
    int quantity;
} player_inventory_t;

typedef struct player_s {
    int pos_x;
    int pos_y;
    enum rotation_e rotation;
    int level;
    int life;
    char *team_name;
    int inventory[COUNT];
    int inventory_size;
    int busy_until;
    char **command_queue;
    int queue_size;
    bool is_in_incantation;
    int incantation_leader_id;
    pending_cmd_t *pending_cmd;
} player_t;

void init_player(player_t *player, char *playerTeam);

// Inventory management functions
void init_inventory(player_t *player);
void free_inventory(player_t *player);

bool add_item_to_inventory(player_t *player, resource_type_t type, int nb);
bool remove_item_from_inventory(player_t *player, resource_type_t t, int nb);

bool inventory_has_item(player_t *player, resource_type_t type);
int how_many_in_inventory(player_t *player, resource_type_t type);

char *get_inventory_content(player_t *player);
char *get_resource_name(resource_type_t type);

#endif /* !PLAYER_H_ */
