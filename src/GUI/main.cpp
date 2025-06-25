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
#include "gameController/EntityFactory.hpp"
#include "shared/services/ComponentCoordinator.hpp"

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
        auto entityFactory = std::make_shared<EntityFactoryManager>();
        // Créer le GameController AVEC l'EntityFactory pour éviter la recréation
        auto gameController = std::make_shared<GameController>(nullptr, entityFactory);
        auto coordinator = std::make_shared<ComponentCoordinator>();
        coordinator->setNetworkManager(networkManager);
        coordinator->setGameController(gameController);
        coordinator->setupConnections();
        auto gameLoop = std::make_shared<GameLoop>(networkManager);
        gameLoop->setServerInfo(parser.getMachine(), parser.getPort());
        gameLoop->setGameController(gameController);
        gameLoop->setComponentCoordinator(coordinator);
        if (!gameLoop->init()) {
            std::cerr << "Failed to initialize game components" << std::endl;
            return 84;
        }
        if (!networkManager->connect(parser.getMachine(), parser.getPort())) {
            std::cerr << "[ERROR] Impossible de se connecter au serveur." << std::endl;
            return 84;
        }
        coordinator->setupConnections();
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
