/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test fork command functionality
*/

#include <criterion/criterion.h>
#include "../../src/Server/include/egg.h"
#include <stdlib.h>
#include <string.h>

Test(fork_basic, create_egg_test)
{
    egg_t *egg = create_egg(1, 42, 5, 10);
    
    cr_assert_not_null(egg);
    cr_assert_eq(egg->egg_id, 1);
    cr_assert_eq(egg->creator_id, 42);
    cr_assert_eq(egg->pos_x, 5);
    cr_assert_eq(egg->pos_y, 10);
    cr_assert_null(egg->next);
    
    free(egg);
}

Test(fork_basic, egg_id_generation)
{
    // Test that we can create multiple eggs with different IDs
    egg_t *egg1 = create_egg(1, 1, 0, 0);
    egg_t *egg2 = create_egg(2, 1, 0, 0);
    
    cr_assert_not_null(egg1);
    cr_assert_not_null(egg2);
    cr_assert_neq(egg1->egg_id, egg2->egg_id);
    
    free(egg1);
    free(egg2);
}
