/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests d'intégration pour NetworkManager
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/logging.h>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstring>
#include <functional>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../src/GUI/network/networkManager/NetworkManager.hpp"
#include "../../src/GUI/shared/exception/AException.hpp"

// Configuration pour rediriger stdout/stderr pour les tests
static void redirect_network_manager_std(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Fonction pour attendre qu'une condition soit remplie
template <typename Predicate>
bool wait_for_condition(Predicate pred, std::chrono::milliseconds timeout = std::chrono::milliseconds(1000)) {
    auto start = std::chrono::steady_clock::now();
    while (!pred()) {
        if (std::chrono::steady_clock::now() - start > timeout) {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return true;
}

// Classe de serveur mock pour tester le NetworkManager
class MockNetworkServer {
public:
    MockNetworkServer(int port = 4242) : _port(port), _running(false), _clientConnected(false) {}

    void start() {
        _running = true;
        _serverThread = std::thread(&MockNetworkServer::run, this);
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

    bool isClientConnected() const {
        return _clientConnected;
    }

    void sendToClient(const std::string& message) {
        if (_clientSocket >= 0 && _clientConnected) {
            send(_clientSocket, message.c_str(), message.size(), 0);
        }
    }

    std::string receiveFromClient(int timeout_ms = 1000) {
        if (_clientSocket < 0 || !_clientConnected) {
            return "";
        }

        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(_clientSocket, &readSet);

        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        int result = select(_clientSocket + 1, &readSet, NULL, NULL, &tv);
        if (result <= 0) {
            return ""; // Timeout ou erreur
        }

        char buffer[1024];
        int bytesRead = recv(_clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            _clientConnected = false;
            return "";
        }

        buffer[bytesRead] = '\0';
        return std::string(buffer, bytesRead);
    }

    bool waitForGraphicCommand(int timeout_ms = 1000) {
        auto start = std::chrono::steady_clock::now();
        while (_running && _clientConnected) {
            std::string message = receiveFromClient(100);
            if (message == "GRAPHIC\n") {
                return true;
            }
            if (std::chrono::steady_clock::now() - start > std::chrono::milliseconds(timeout_ms)) {
                return false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return false;
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
            listen(_serverSocket, 3) < 0) {
            close(_serverSocket);
            _serverSocket = -1;
            return;
        }

        while (_running) {
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(_serverSocket, &readSet);

            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 100000; // 100ms timeout

            int activity = select(_serverSocket + 1, &readSet, NULL, NULL, &tv);
            if (activity <= 0) {
                continue;
            }

            if (FD_ISSET(_serverSocket, &readSet)) {
                struct sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(clientAddr);
                _clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
                if (_clientSocket >= 0) {
                    _clientConnected = true;
                }
            }
        }
    }

    int _port;
    std::atomic<bool> _running;
    std::atomic<bool> _clientConnected;
    std::thread _serverThread;
    int _serverSocket = -1;
    int _clientSocket = -1;
};

// Tests du NetworkManager

Test(NetworkManager, ConnectionSuccess, .init = redirect_network_manager_std) {
    MockNetworkServer server(4247);
    server.start();
    
    NetworkManager networkManager;
    bool connected = networkManager.connect("127.0.0.1", 4247);
    
    cr_assert(connected, "NetworkManager should connect successfully");
    cr_assert(networkManager.isConnected(), "NetworkManager should report being connected");
    
    // Attendre que le serveur détecte la connexion
    bool serverDetectedClient = wait_for_condition([&server]() {
        return server.isClientConnected();
    });
    
    cr_assert(serverDetectedClient, "Server should detect client connection");
    
    networkManager.disconnect();
    server.stop();
}

Test(NetworkManager, SendGraphicCommand, .init = redirect_network_manager_std) {
    MockNetworkServer server(4248);
    server.start();
    
    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", 4248);
    
    // Attendre que le serveur accepte la connexion
    bool clientConnected = wait_for_condition([&server]() {
        return server.isClientConnected();
    });
    cr_assert(clientConnected, "Server should accept connection");
    
    // Envoyer la commande GRAPHIC
    networkManager.sendCommand("GRAPHIC\n");
    
    // Vérifier que le serveur a bien reçu la commande GRAPHIC
    bool graphicCommandReceived = server.waitForGraphicCommand();
    cr_assert(graphicCommandReceived, "Server should receive GRAPHIC command");
    
    networkManager.disconnect();
    server.stop();
}

Test(NetworkManager, ReceiveAndProcessMessages, .init = redirect_network_manager_std) {
    MockNetworkServer server(4249);
    server.start();
    
    NetworkManager networkManager;
    networkManager.connect("127.0.0.1", 4249);
    
    // Attendre que le serveur accepte la connexion
    bool clientConnected = wait_for_condition([&server]() {
        return server.isClientConnected();
    });
    cr_assert(clientConnected, "Server should accept connection");
    
    // Le serveur envoie un message
    server.sendToClient("msz 10 20\n");
    
    // Donner le temps au NetworkManager de traiter le message
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Traiter les messages entrants
    networkManager.processIncomingMessages();
    
    // Note: Pour un vrai test, il faudrait avoir accès à l'état interne du NetworkManager
    // ou avoir un moyen de vérifier que le message a été correctement traité
    
    networkManager.disconnect();
    server.stop();
}

Test(NetworkManager, ConnectionErrorHandling, .init = redirect_network_manager_std) {
    NetworkManager networkManager;
    
    // Essayer de se connecter à un port non ouvert
    bool connected = networkManager.connect("127.0.0.1", 9999);
    
    cr_assert(!connected, "Connection to non-existent server should fail");
    cr_assert(!networkManager.isConnected(), "NetworkManager should report not being connected");
}
