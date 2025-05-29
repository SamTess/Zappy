/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Main
*/

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "parsing/ParsingCLI.hpp"
#include "network/networkManager/NetworkManager.hpp"
#include "network/networkManager/NetworkLogger.hpp"

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
        NetworkLogger::get().setEnabled(true);
        NetworkManager networkManager;
        if (!networkManager.connect(parser.getMachine(), parser.getPort())) {
            std::cerr << "[ERROR] Impossible de se connecter au serveur." << std::endl;
            return 84;
        }
        NetworkLogger::get().log("Connexion établie. Envoi de GRAPHIC\\n...");
        networkManager.sendCommand("GRAPHIC\n");
        while (networkManager.isConnected()) {
            networkManager.processIncomingMessages();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cerr << "[INFO] Déconnecté du serveur." << std::endl;
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
