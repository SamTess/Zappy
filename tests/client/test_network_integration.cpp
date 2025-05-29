/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests d'intégration des composants réseau
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/logging.h>
#include <string>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../src/GUI/network/buffer/CircularBuffer.hpp"
#include "../../src/GUI/network/buffer/MessageQueue.hpp"
#include "../../src/GUI/network/connection/TcpConnection.hpp"
#include "../../src/GUI/network/protocol/ProtocolParser.hpp"
#include "../../src/GUI/network/networkManager/NetworkManager.hpp"
#include "../../src/GUI/network/networkManager/NetworkLogger.hpp"
#include "../../src/GUI/shared/exception/AException.hpp"

// Classe de serveur mock avancé pour les tests d'intégration
class IntegrationTestServer {
public:
    IntegrationTestServer(int port = 4245) : _port(port), _running(false) {}

    void start() {
        _running = true;
        _serverThread = std::thread(&IntegrationTestServer::run, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

    bool receivedGraphicCommand() const {
        return _receivedGraphic;
    }

    void sendServerCommands() {
        if (_clientSocket >= 0) {
            const char *commands[] = {
                "msz 10 20\n",
                "bct 0 0 1 2 3 4 5 6 7\n",
                "tna Team1\n",
                "tna Team2\n",
                "pnw 1 0 0 1 1 Team1\n"
            };

            for (const char *cmd : commands) {
                send(_clientSocket, cmd, strlen(cmd), 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
    }

    void waitForConnection() {
        while (_running && _clientSocket < 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    ~IntegrationTestServer() {
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

        // Configurer un timeout pour accept
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        fd_set readfds;

        while (_running) {
            FD_ZERO(&readfds);
            FD_SET(_serverSocket, &readfds);

            int activity = select(_serverSocket + 1, &readfds, NULL, NULL, &tv);
            
            if (activity < 0) {
                continue;
            }

            if (FD_ISSET(_serverSocket, &readfds)) {
                struct sockaddr_in clientAddr;
                socklen_t addrlen = sizeof(clientAddr);
                
                _clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &addrlen);
                if (_clientSocket < 0) {
                    continue;
                }

                // Attendre et lire les commandes du client
                char buffer[1024] = {0};
                int valread = read(_clientSocket, buffer, 1024);
                
                if (valread > 0) {
                    if (strncmp(buffer, "GRAPHIC\n", 8) == 0) {
                        _receivedGraphic = true;
                    }
                }
            }
        }
    }

    int _port;
    int _serverSocket = -1;
    int _clientSocket = -1;
    bool _running;
    bool _receivedGraphic = false;
    std::thread _serverThread;
};

// Fixture pour rediriger stdout et stderr
void redirect_all_output(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test d'intégration complet
Test(network_full_integration, connection_and_message_flow, .init=redirect_all_output) {
    // Démarrer un serveur de test
    IntegrationTestServer server(4245);
    server.start();
    
    // Logger en mode debug
    NetworkLogger::get().setEnabled(true);
    
    // Initialiser et connecter NetworkManager
    NetworkManager networkManager;
    bool connected = networkManager.connect("127.0.0.1", 4245);
    cr_assert(connected, "NetworkManager should connect successfully");
    
    // Envoyer la commande GRAPHIC
    networkManager.sendCommand("GRAPHIC\n");
    
    // Attendre que le serveur traite la connexion
    server.waitForConnection();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Vérifier que le serveur a reçu GRAPHIC
    cr_assert(server.receivedGraphicCommand(), "Server should receive GRAPHIC command");
    
    // Le serveur envoie des commandes de protocole
    server.sendServerCommands();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // Traiter les messages reçus
    networkManager.processIncomingMessages();
    
    // Déconnexion propre
    networkManager.disconnect();
    cr_assert_not(networkManager.isConnected(), "NetworkManager should be disconnected");
    
    server.stop();
}

// Test avec reconnexion
Test(network_full_integration, reconnection, .init=redirect_all_output) {
    IntegrationTestServer server1(4246);
    server1.start();
    
    NetworkLogger::get().setEnabled(true);
    NetworkManager networkManager;
    
    // Première connexion
    bool connected1 = networkManager.connect("127.0.0.1", 4246);
    cr_assert(connected1, "First connection should succeed");
    networkManager.sendCommand("GRAPHIC\n");
    
    // Attendre et traiter
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    networkManager.processIncomingMessages();
    
    // Arrêter le premier serveur
    server1.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // La connexion devrait être perdue à ce stade
    networkManager.disconnect();
    
    // Démarrer un nouveau serveur
    IntegrationTestServer server2(4246);
    server2.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Reconnecter
    bool connected2 = networkManager.connect("127.0.0.1", 4246);
    cr_assert(connected2, "Reconnection should succeed");
    networkManager.sendCommand("GRAPHIC\n");
    
    // Attendre et vérifier
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    server2.waitForConnection();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    cr_assert(server2.receivedGraphicCommand(), "Second server should receive GRAPHIC command");
    
    // Nettoyage final
    networkManager.disconnect();
    server2.stop();
}

// Test d'erreur réseau
Test(network_full_integration, network_error_handling, .init=redirect_all_output) {
    NetworkLogger::get().setEnabled(true);
    NetworkManager networkManager;
    
    // Tenter une connexion vers un port fermé
    bool connected = networkManager.connect("127.0.0.1", 65432);
    cr_assert_not(connected, "Connection to closed port should fail");
    
    // Tenter d'envoyer une commande sans être connecté
    networkManager.sendCommand("GRAPHIC\n");
    // Pas de crash attendu
    
    // Tenter de traiter des messages sans être connecté
    networkManager.processIncomingMessages();
    // Pas de crash attendu
}

// Fonction simulant l'utilisation des composants dans le main
bool simulateMainFlow() {
    try {
        // Créer un logger et l'activer
        NetworkLogger::get().setEnabled(true);
        
        // Créer un gestionnaire réseau
        NetworkManager networkManager;
        
        // Tenter de se connecter (échouera car pas de serveur)
        if (!networkManager.connect("127.0.0.1", 65433)) {
            cr_log_info("Connection failed as expected");
            return true;
        }
        
        return false;
    } catch (...) {
        return false;
    }
}

// Test simulant le flux principal
Test(network_full_integration, main_flow_simulation, .init=redirect_all_output) {
    bool result = simulateMainFlow();
    cr_assert(result, "Main flow simulation should handle connection failure gracefully");
}
