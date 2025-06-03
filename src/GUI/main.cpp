/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Main
*/

#include <iostream>
#include <string>
#include "parsing/ParsingCLI.hpp"
#include "GameLoop.hpp"

void displayHelp() {
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
    std::cout << "\tport\tis the port number" << std::endl;
    std::cout << "\tmachine\tis the name of the machine; localhost by default" << std::endl;
}

int main(int argc, char** argv) {
    try {
        if (argc == 2 && std::string(argv[1]) == "--help") {
            displayHelp();
            return 0;
        }
        ParsingCLI parser(argc, argv);
        std::string host = parser.getMachine();
        int port = parser.getPort();
        if (port <= 0 || port > 65535) {
            std::cerr << "Port number must be between 1 and 65535" << std::endl;
            return 84;
        }
        GameLoop gameLoop;
        gameLoop.setServerInfo(host, port);
        if (!gameLoop.init()) {
            std::cerr << "Failed to initialize game components" << std::endl;
            return 84;
        }
        return gameLoop.run();
    } catch (const AException &e) {
        std::cerr << e.getFormattedMessage() << std::endl;
        return 84;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
        return 84;
    }
    return 0;
}
