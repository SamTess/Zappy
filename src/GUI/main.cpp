/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Main
*/

#include <iostream>
#include <string>
#include <cstring>

void display_help();

/**
 * @brief Main function for the zappy GUI client
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return 0 on success, 84 on error
 */
int main(int argc, char **argv) {
    if ((argc == 2 && (strcmp(argv[1], "-help") == 0 ||
        strcmp(argv[1], "help") == 0)) || argc == 1) {
        display_help();
        return 0;
    }
    return 0;
}
