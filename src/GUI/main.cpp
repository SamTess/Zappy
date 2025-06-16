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
#include "logger/Logger.hpp"

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

        std::shared_ptr<Logger> logger = std::make_shared<Logger>("zappy_gui.log", true);
        logger->logInfo("=== Zappy GUI Started ===");
        logger->logInfo("Connecting to " + parser.getMachine() + " on port " + std::to_string(parser.getPort()));
        NetworkManager networkManager(logger);

        if (!networkManager.connect(parser.getMachine(), parser.getPort())) {
            logger->logError("Failed to connect to server");
            std::cerr << "[ERROR] Impossible de se connecter au serveur." << std::endl;
            return 84;
        }
        logger->logInfo("Connected to server successfully");
        auto gameLoop = std::make_shared<GameLoop>(logger);
        gameLoop->setServerInfo(parser.getMachine(), parser.getPort());
        gameLoop->setGameController(networkManager.getGameController());
        if (!gameLoop->init()) {
            logger->logError("Failed to initialize game components");
            std::cerr << "Failed to initialize game components" << std::endl;
            return 84;
        }
        logger->logInfo("Starting game loop");
        gameLoop->run();
        logger->logInfo("Game loop ended");
        networkManager.disconnect();
        logger->logInfo("=== Zappy GUI Ended ===");
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
