/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Main
*/

#include <iostream>
#include <string>
#include <memory>
#include "parsing/ParsingCLI.hpp"
#include "GameLoop.hpp"
#include "network/networkManager/NetworkManager.hpp"

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
        std::cout << "Connecting to " << parser.getMachine() << " on port " << parser.getPort() << std::endl;

        auto networkManager = std::make_shared<NetworkManager>();

        if (!networkManager->connect(parser.getMachine(), parser.getPort())) {
            std::cerr << "[ERROR] Impossible de se connecter au serveur." << std::endl;
            return 84;
        }

        auto gameController = std::make_shared<GameController>(networkManager);
        networkManager->setGameController(gameController);

        auto gameLoop = std::make_shared<GameLoop>(networkManager);
        gameLoop->setServerInfo(parser.getMachine(), parser.getPort());
        gameLoop->setGameController(gameController);
        if (!gameLoop->init()) {
            std::cerr << "Failed to initialize game components" << std::endl;
            return 84;
        }
        gameLoop->run();
        networkManager->disconnect();
        return 0;
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
