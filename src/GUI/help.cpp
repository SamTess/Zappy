/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Help message
*/

#include <iostream>

/**
 * @brief Display the help message for the GUI client
 */
void display_help() {
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
    std::cout << "\t-p port      port number" << std::endl;
    std::cout << "\t-h machine   hostname of the server" << std::endl;
}

