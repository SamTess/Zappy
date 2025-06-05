/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg
*/

#ifndef EGG_H_
    #define EGG_H_

typedef struct server_s server_t;
typedef struct tile_s tile_t;

typedef struct egg_s {
    int egg_id;
    int creator_id;
    int pos_x;
    int pos_y;
    struct egg_s *next;
} egg_t;

egg_t *create_egg(int egg_id, int creator_id, int pos_x, int pos_y);
void remove_egg(server_t *server, int id, tile_t *tile);
void add_egg(server_t *server, egg_t *egg);

#endif /* !EGG_H_ */
