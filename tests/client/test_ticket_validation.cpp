/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests d'intégration pour vérifier les exigences du ticket
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/logging.h>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <cstring>
#include <functional>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include "../../src/GUI/network/connection/TcpConnection.hpp"
#include "../../src/GUI/network/networkManager/NetworkManager.hpp"
#include "../../src/GUI/network/networkManager/NetworkLogger.hpp"
#include "../../src/GUI/network/buffer/CircularBuffer.hpp"
#include "../../src/GUI/network/buffer/MessageQueue.hpp"
#include "../../src/GUI/shared/exception/AException.hpp"

// Configuration pour rediriger stdout/stderr pour les tests
static void redirect_ticket_validation_std(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Serveur mock qui implémente les comportements attendus du serveur Zappy
class ZappyMockServer {
public:
    ZappyMockServer(int port = 4242) : _port(port), _running(false) {
        _receivedCommands.clear();
    }

    void start() {
        _running = true;
        _serverThread = std::thread(&ZappyMockServer::run, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre le démarrage du serveur
    }

    void stop() {
        _running = false;
        if (_serverThread.joinable()) {
            _serverThread.join();
        }
        
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (int clientSocket : _clientSockets) {
            if (clientSocket >= 0) {
                close(clientSocket);
            }
        }
        
        if (_serverSocket >= 0) {
            close(_serverSocket);
        }
    }

    bool hasReceivedGraphicCommand() {
        std::lock_guard<std::mutex> lock(_commandsMutex);
        for (const auto& cmd : _receivedCommands) {
            if (cmd == "GRAPHIC\n") {
                return true;
            }
        }
        return false;
    }

    void sendToAllClients(const std::string& message) {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (int clientSocket : _clientSockets) {
            if (clientSocket >= 0) {
                send(clientSocket, message.c_str(), message.size(), 0);
            }
        }
    }

    std::vector<std::string> getReceivedCommands() {
        std::lock_guard<std::mutex> lock(_commandsMutex);
        return _receivedCommands;
    }

    bool hasActiveClients() {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        return !_clientSockets.empty();
    }

private:
    void run() {
        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (_serverSocket < 0) {
            return;
        }

        int opt = 1;
        setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(_port);

        if (bind(_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0 ||
            listen(_serverSocket, 5) < 0) {
            close(_serverSocket);
            _serverSocket = -1;
            return;
        }

        while (_running) {
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(_serverSocket, &readSet);

            int maxFd = _serverSocket;

            {
                std::lock_guard<std::mutex> lock(_clientsMutex);
                for (int clientSocket : _clientSockets) {
                    if (clientSocket > 0) {
                        FD_SET(clientSocket, &readSet);
                    }
                    maxFd = std::max(maxFd, clientSocket);
                }
            }

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 100000; // 100ms timeout

            int activity = select(maxFd + 1, &readSet, NULL, NULL, &tv);
            if (activity < 0) {
                continue;
            }

            if (FD_ISSET(_serverSocket, &readSet)) {
                struct sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(clientAddr);
                int newSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
                
                if (newSocket >= 0) {
                    // Mettre le socket en mode non-bloquant
                    int flags = fcntl(newSocket, F_GETFL, 0);
                    fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);

                    std::lock_guard<std::mutex> lock(_clientsMutex);
                    _clientSockets.push_back(newSocket);
                    
                    // Envoyer immédiatement des données de test au client
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    sendInitialMessages(newSocket);
                }
            }

            std::lock_guard<std::mutex> lock(_clientsMutex);
            for (auto it = _clientSockets.begin(); it != _clientSockets.end();) {
                int clientSocket = *it;
                if (FD_ISSET(clientSocket, &readSet)) {
                    char buffer[1024] = {0};
                    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                    
                    if (bytesRead > 0) {
                        std::string command(buffer, bytesRead);
                        std::lock_guard<std::mutex> cmdLock(_commandsMutex);
                        _receivedCommands.push_back(command);
                        
                        // Si on reçoit GRAPHIC, répondre avec les commandes du protocole
                        if (command == "GRAPHIC\n") {
                            sendProtocolResponses(clientSocket);
                        }
                    } else if (bytesRead <= 0) {
                        close(clientSocket);
                        it = _clientSockets.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }
    }

    void sendInitialMessages(int /* clientSocket */) {
        // Rien à envoyer initialement
    }

    void sendProtocolResponses(int clientSocket) {
        // Simuler les messages du serveur après réception de GRAPHIC
        std::vector<std::string> responses = {
            "msz 10 20\n",
            "sgt 100\n",
            "bct 0 0 1 2 3 4 5 6 7\n",
            "tna Team1\n",
            "tna Team2\n"
        };
        
        for (const auto& response : responses) {
            send(clientSocket, response.c_str(), response.size(), 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    int _port;
    std::atomic<bool> _running;
    std::thread _serverThread;
    int _serverSocket = -1;
    std::vector<int> _clientSockets;
    std::vector<std::string> _receivedCommands;
    std::mutex _clientsMutex;
    std::mutex _commandsMutex;
};

// Fonction d'attente conditionnelle avec timeout
template <typename Predicate>
bool wait_for_condition_timeout(Predicate pred, std::chrono::milliseconds timeout = std::chrono::milliseconds(2000)) {
    auto start = std::chrono::steady_clock::now();
    while (!pred()) {
        if (std::chrono::steady_clock::now() - start > timeout) {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return true;
}

// === Tests de validation du ticket ===

Test(TicketValidation, TcpSocketOpeningAndConnection, .init = redirect_ticket_validation_std) {
    // Partie 1: Ouverture du socket TCP
    ZappyMockServer server(4251);
    server.start();
    
    TcpConnection connection;
    connection.connect("127.0.0.1", 4251);
    cr_assert(connection.isConnected(), "Socket TCP devrait être correctement ouvert et connecté");
    
    connection.close();
    server.stop();
}

Test(TicketValidation, GraphicCommandSending, .init = redirect_ticket_validation_std) {
    // Partie 2: Envoi immédiat de GRAPHIC\n
    ZappyMockServer server(4252);
    server.start();
    
    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", 4252);
    networkManager.sendCommand("GRAPHIC\n");
    
    bool graphicReceived = wait_for_condition_timeout([&server]() {
        return server.hasReceivedGraphicCommand();
    });
    
    cr_assert(graphicReceived, "La commande GRAPHIC\\n devrait être envoyée et reçue par le serveur");
    
    networkManager.disconnect();
    server.stop();
}

Test(TicketValidation, AsynchronousReception, .init = redirect_ticket_validation_std) {
    // Partie 3: Réception asynchrone
    ZappyMockServer server(4253);
    server.start();
    
    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", 4253);
    networkManager.sendCommand("GRAPHIC\n");
    
    // Attendre que le serveur reçoive la commande GRAPHIC
    bool graphicReceived = wait_for_condition_timeout([&server]() {
        return server.hasReceivedGraphicCommand();
    });
    cr_assert(graphicReceived, "La commande GRAPHIC\\n devrait être envoyée");
    
    // Le serveur va envoyer automatiquement plusieurs réponses après réception de GRAPHIC
    // Attendre un peu pour laisser le temps au NetworkManager de traiter les données
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Traiter les messages reçus
    networkManager.processIncomingMessages();
    
    // Note: Pour un test complet, il faudrait pouvoir vérifier les messages reçus
    // ce qui nécessiterait un accès à l'état interne du NetworkManager ou un callback
    
    networkManager.disconnect();
    server.stop();
}

Test(TicketValidation, NonBlockingReception, .init = redirect_ticket_validation_std) {
    // Partie 4: Lecture non-bloquante des données
    CircularBuffer buffer(1024);
    
    // Écrire des données
    const char* testData = "msz 10 20\nbct 0 0 1 2 3 4 5 6 7\n";
    buffer.write(testData, strlen(testData));
    
    // Lire des données sans bloquer
    char readBuffer[256] = {0};
    size_t bytesRead = buffer.read(readBuffer, sizeof(readBuffer));
    
    cr_assert_eq(bytesRead, strlen(testData), "Le buffer de réception devrait lire les données correctement");
    cr_assert_str_eq(readBuffer, testData, "Les données lues devraient correspondre aux données écrites");
}

Test(TicketValidation, ErrorHandling, .init = redirect_ticket_validation_std) {
    // Partie 5: Gestion des erreurs
    NetworkManager networkManager;
    
    // Tentative de connexion à un serveur inexistant
    bool connected = networkManager.connect("127.0.0.1", 9999);
    cr_assert(!connected, "La connexion à un serveur inexistant devrait échouer");
    cr_assert(!networkManager.isConnected(), "NetworkManager devrait indiquer qu'il n'est pas connecté après un échec");
    
    // Connexion à un vrai serveur puis fermeture du serveur
    ZappyMockServer server(4254);
    server.start();
    
    connected = networkManager.connect("127.0.0.1", 4254);
    cr_assert(connected, "Devrait se connecter au serveur mock");
    
    // Arrêter le serveur pour simuler une déconnexion
    server.stop();
    
    // Essayer d'envoyer quelque chose après la déconnexion
    networkManager.sendCommand("GRAPHIC\n");
    
    // À ce stade, le NetworkManager devrait détecter la déconnexion
    // et gérer l'erreur sans crasher
}

Test(TicketValidation, BufferStorage, .init = redirect_ticket_validation_std) {
    // Partie 6: Stockage dans un buffer
    MessageQueue messageQueue;
    messageQueue.enqueue("msz 10 20\n");
    messageQueue.enqueue("bct 0 0 1 2 3 4 5 6 7\n");
    
    cr_assert(!messageQueue.isEmpty(), "La file de messages ne devrait pas être vide après enqueue");
    
    std::string message1 = messageQueue.dequeue();
    std::string message2 = messageQueue.dequeue();
    
    cr_assert_str_eq(message1.c_str(), "msz 10 20\n", "Premier message récupéré incorrect");
    cr_assert_str_eq(message2.c_str(), "bct 0 0 1 2 3 4 5 6 7\n", "Deuxième message récupéré incorrect");
    cr_assert(messageQueue.isEmpty(), "La file de messages devrait être vide après avoir tout récupéré");
}

Test(TicketValidation, NetworkLogging, .init = redirect_ticket_validation_std) {
    // Partie 7: Logging réseau
    NetworkLogger::get().setEnabled(true);
    
    // Le message devrait s'afficher sur stdout
    NetworkLogger::get().log("Test de logging réseau");
    
    // Désactiver le logger
    NetworkLogger::get().setEnabled(false);
    NetworkLogger::get().log("Ce message ne devrait pas s'afficher");
    
    // Réactiver pour vérifier l'état
    cr_assert(!NetworkLogger::get().isEnabled(), "Le logger devrait être désactivé");
}

// Test global qui vérifie l'ensemble des fonctionnalités du ticket
Test(TicketValidation, CompleteWorkflow, .init = redirect_ticket_validation_std, .timeout = 5) {
    ZappyMockServer server(4255);
    server.start();
    
    NetworkManager networkManager;
    
    // 1. Connexion au serveur
    bool connected = networkManager.connect("127.0.0.1", 4255);
    cr_assert(connected, "NetworkManager devrait se connecter au serveur");
    
    // 2. Envoi de GRAPHIC
    networkManager.sendCommand("GRAPHIC\n");
    
    // 3. Attendre que le serveur reçoive GRAPHIC
    bool graphicReceived = wait_for_condition_timeout([&server]() {
        return server.hasReceivedGraphicCommand();
    });
    cr_assert(graphicReceived, "Le serveur devrait recevoir la commande GRAPHIC\\n");
    
    // 4. Attendre que le serveur réponde avec des commandes
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // 5. Traiter les messages reçus
    networkManager.processIncomingMessages();
    
    // 6. Fermeture de la connexion
    networkManager.disconnect();
    server.stop();
    
    // Le test passe si aucune exception ou assertion n'échoue
}
