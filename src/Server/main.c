/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Server main file
*/

#include <stdio.h>
#include <string.h>
#include "include/server.h"

/**
 * @brief Main function for the zappy server
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return 0 on success, 84 on error
 */
int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "-help") == 0
    || strcmp(argv[1], "help") == 0)) {
        display_help();
        return 0;
    }
    return 0;
}
