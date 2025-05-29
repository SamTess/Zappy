/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests pour le NetworkManager
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/logging.h>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../src/GUI/network/networkManager/NetworkManager.hpp"
#include "../../src/GUI/shared/exception/AException.hpp"

// Structure de serveur mock pour les tests
class TestServer {
public:
    TestServer(int port = 4242) : _port(port), _running(false) {}

    void start() {
        _running = true;
        _serverThread = std::thread(&TestServer::run, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Donner le temps au serveur de démarrer
    }

    void stop() {
        _running = false;
        if (_serverThread.joinable()) {
            _serverThread.join();
        }
        if (_clientSocket >= 0) {
            close(_clientSocket);
        }
        if (_serverSocket >= 0) {
            close(_serverSocket);
        }
    }

    bool sentGraphicCommand() const {
        return _receivedGraphic;
    }

    ~TestServer() {
        stop();
    }

private:
    void run() {
        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (_serverSocket == -1) {
            cr_log_error("Failed to create server socket");
            return;
        }

        int opt = 1;
        setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(_port);

        if (bind(_serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
            cr_log_error("Bind failed");
            close(_serverSocket);
            return;
        }

        if (listen(_serverSocket, 3) < 0) {
            cr_log_error("Listen failed");
            close(_serverSocket);
            return;
        }

        struct sockaddr_in clientAddr;
        socklen_t addrlen = sizeof(clientAddr);
        
        _clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &addrlen);
        if (_clientSocket < 0) {
            cr_log_error("Accept failed");
            close(_serverSocket);
            return;
        }

        char buffer[1024] = {0};
        int valread = read(_clientSocket, buffer, 1024);
        
        if (valread > 0) {
            cr_log_info("Server received: %s", buffer);
            if (strncmp(buffer, "GRAPHIC\n", 8) == 0) {
                _receivedGraphic = true;
                const char *response = "msz 10 10\n";
                send(_clientSocket, response, strlen(response), 0);
            }
        }

        while (_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    int _port;
    int _serverSocket = -1;
    int _clientSocket = -1;
    bool _running;
    bool _receivedGraphic = false;
    std::thread _serverThread;
};

// Fixture pour rediriger stderr
void redirect_all_stdout(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Tests pour NetworkManager
Test(network_manager, initialization) {
    NetworkManager manager;
    cr_assert_not(manager.isConnected(), "New manager should not be connected");
}

Test(network_manager, connect, .init=redirect_all_stdout) {
    TestServer server(4244);
    server.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre que le serveur démarre
    
    NetworkManager manager;
    bool connected = manager.connect("127.0.0.1", 4244);
    
    cr_assert(connected, "Manager should connect successfully");
    cr_assert(manager.isConnected(), "Manager should report as connected");
    
    // Envoyer la commande GRAPHIC
    manager.sendCommand("GRAPHIC\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre que le serveur traite
    
    // Traiter les messages entrants
    manager.processIncomingMessages();
    
    // Vérifier que le serveur a bien reçu la commande GRAPHIC
    cr_assert(server.sentGraphicCommand(), "Server should receive GRAPHIC command");
    
    manager.disconnect();
    cr_assert_not(manager.isConnected(), "Manager should be disconnected");
    
    server.stop();
}

Test(network_manager, connect_error, .init=redirect_all_stdout) {
    NetworkManager manager;
    bool connected = manager.connect("127.0.0.1", 65432); // Port probablement inutilisé
    
    cr_assert_not(connected, "Manager should fail to connect to invalid port");
    cr_assert_not(manager.isConnected(), "Manager should not be connected after failure");
}

Test(network_manager, send_error, .init=redirect_all_stdout) {
    NetworkManager manager;
    
    // Tenter d'envoyer sans être connecté
    manager.sendCommand("GRAPHIC\n");
    // Pas de crash attendu, mais aucun envoi ne devrait être effectué
}

Test(network_manager, disconnect_when_not_connected, .init=redirect_all_stdout) {
    NetworkManager manager;
    
    // Appeler disconnect sans être connecté ne devrait pas causer d'erreur
    manager.disconnect();
    cr_assert_not(manager.isConnected(), "Manager should remain disconnected");
}
