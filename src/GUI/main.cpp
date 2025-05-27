/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Main
*/

#include <iostream>
#include <string>
#include "parsing/ParsingCLI.hpp"

void display_help();

/**
 * @brief Main function for the zappy GUI client
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return 0 on success, 84 on error
 */
int main(int argc, char **argv) {

    try {
        ParsingCLI parser(argc, argv);
        std::cout << "Connecting to " << parser.getMachine() << " on port " << parser.getPort() << std::endl;

    } catch (const AException &e) {
        std::cerr << e.getFormattedMessage() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
        return 84;
    }
    return 0;
}
