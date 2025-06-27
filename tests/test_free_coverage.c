/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests for free.c - Coverage improvement (fixed version)
*/

#include <criterion/criterion.h>
#include "../src/Server/include/server.h"

Test(free, free_all_basic)
{
    // Test that free_all doesn't crash with NULL
    server_t *server = NULL;
    parsing_info_t *parsed_info = NULL;
    
    // Should not crash
    free_all(server, parsed_info);
    cr_assert(true); // If we reach here, no crash occurred
}

Test(free, free_arr_basic)
{
    // Test that free_arr doesn't crash with NULL
    char **arr = NULL;
    
    // Should not crash
    free_arr(arr);
    cr_assert(true); // If we reach here, no crash occurred
}