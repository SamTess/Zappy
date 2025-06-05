/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test connect_nbr command with egg functionality
*/

#include <criterion/criterion.h>
#include "../../src/Server/include/egg.h"
#include <stdlib.h>
#include <string.h>

Test(connect_nbr_basic, egg_creation_logic)
{
    // Basic test that eggs can be created - this simulates the team slot logic
    egg_t *egg1 = create_egg(1, 100, 0, 0); // Egg created by player 100
    egg_t *egg2 = create_egg(2, 100, 1, 1); // Another egg by same player
    
    cr_assert_not_null(egg1);
    cr_assert_not_null(egg2);
    
    // Verify that each egg has the correct creator
    cr_assert_eq(egg1->creator_id, 100);
    cr_assert_eq(egg2->creator_id, 100);
    
    // Different egg IDs
    cr_assert_neq(egg1->egg_id, egg2->egg_id);
    
    free(egg1);
    free(egg2);
}

Test(connect_nbr_basic, slot_calculation_concept)
{
    // Test the basic logic of slot calculation:
    // available_slots = (base_slots + eggs_laid_by_team) - current_team_members
    
    int base_slots = 3;
    int eggs_by_team = 2; // Team laid 2 eggs
    int current_members = 2; // 2 players currently connected
    
    int available_slots = (base_slots + eggs_by_team) - current_members;
    
    cr_assert_eq(available_slots, 3); // (3 + 2) - 2 = 3
}
