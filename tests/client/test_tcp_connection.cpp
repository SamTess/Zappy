/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour TcpConnection
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
#include <functional>
#include "../../src/GUI/network/connection/TcpConnection.hpp"
#include "../../src/GUI/shared/exception/AException.hpp"

// Structure de serveur mock pour les tests
struct MockTcpServer {
    int serverSocket;
    int clientSocket;
    struct sockaddr_in address;
    bool isRunning;
    std::thread serverThread;

    MockTcpServer(int port = 4242) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            throw std::runtime_error("Failed to create server socket");
        }

        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            throw std::runtime_error("Failed to set socket options");
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
            throw std::runtime_error("Failed to bind server socket");
        }

        if (listen(serverSocket, 3) < 0) {
            throw std::runtime_error("Failed to listen on server socket");
        }

        isRunning = true;
        clientSocket = -1;
    }

    ~MockTcpServer() {
        stop();
    }

    void start() {
        serverThread = std::thread([this]() {
            socklen_t addrlen = sizeof(address);
            clientSocket = accept(serverSocket, (struct sockaddr *)&address, &addrlen);
            
            while (isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    void stop() {
        isRunning = false;
        if (serverThread.joinable()) {
            serverThread.join();
        }
        if (clientSocket >= 0) {
            close(clientSocket);
        }
        if (serverSocket >= 0) {
            close(serverSocket);
        }
    }

    void sendData(const std::string &data) {
        if (clientSocket >= 0) {
            send(clientSocket, data.c_str(), data.size(), 0);
        }
    }

    std::string receiveData() {
        if (clientSocket < 0) {
            return "";
        }

        char buffer[1024] = {0};
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
        
        if (bytesReceived > 0) {
            return std::string(buffer, bytesReceived);
        }
        
        return "";
    }
};

// Configuration pour rediriger stdout/stderr pour les tests
static void redirect_all_std(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Fonction pour attendre qu'une action se produise avant de continuer (avec timeout)
bool wait_for(std::function<bool()> condition, std::chrono::milliseconds timeout = std::chrono::milliseconds(1000)) {
    auto start = std::chrono::steady_clock::now();
    while (!condition()) {
        if (std::chrono::steady_clock::now() - start > timeout) {
            return false; // Timeout atteint
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return true; // Condition remplie
}

Test(TcpConnection, BasicConstructorDestructor) {
    TcpConnection connection;
    cr_assert(!connection.isConnected(), "New TcpConnection should not be connected");
}

Test(TcpConnection, ConnectToServer, .init = redirect_all_std) {
    MockTcpServer server(4243);
    server.start();
    
    TcpConnection connection;
    connection.connect("127.0.0.1", 4243);
    
    cr_assert(connection.isConnected(), "TcpConnection should be connected after connect");
    
    // Attendre que la connexion soit acceptée par le serveur mock
    bool connected = wait_for([&server]() { return server.clientSocket >= 0; });
    cr_assert(connected, "Server failed to accept client connection");
    
    connection.close();
    server.stop();
}

Test(TcpConnection, SendMessage, .init = redirect_all_std) {
    MockTcpServer server(4244);
    server.start();
    
    TcpConnection connection;
    connection.connect("127.0.0.1", 4244);
    
    // Attendre que la connexion soit acceptée
    bool connected = wait_for([&server]() { return server.clientSocket >= 0; });
    cr_assert(connected, "Server failed to accept client connection");
    
    // Envoyer un message au serveur
    std::string testMessage = "GRAPHIC\n";
    connection.send(testMessage);
    
    // Attendre et vérifier que le serveur a reçu le message
    bool received = wait_for([&server, &testMessage]() {
        std::string data = server.receiveData();
        return data == testMessage;
    });
    
    cr_assert(received, "Server did not receive the message correctly");
    
    connection.close();
    server.stop();
}

Test(TcpConnection, ReceiveMessage, .init = redirect_all_std) {
    MockTcpServer server(4245);
    server.start();
    
    TcpConnection connection;
    connection.connect("127.0.0.1", 4245);
    
    // Attendre que la connexion soit acceptée
    bool connected = wait_for([&server]() { return server.clientSocket >= 0; });
    cr_assert(connected, "Server failed to accept client connection");
    
    // Le serveur envoie un message au client
    std::string testMessage = "msz 10 20\n";
    server.sendData(testMessage);
    
    // Attendre que les données soient disponibles
    bool dataAvailable = wait_for([&connection]() {
        return connection.hasData();
    });
    
    cr_assert(dataAvailable, "Data should be available after server sends message");
    
    // Récupérer et vérifier le message
    std::string receivedMessage = connection.receive();
    cr_assert_eq(receivedMessage, testMessage, "Received message does not match sent message");
    
    connection.close();
    server.stop();
}

Test(TcpConnection, ConnectionError, .init = redirect_all_std) {
    TcpConnection connection;
    
    try {
        // Essayer de se connecter à un port non ouvert
        connection.connect("127.0.0.1", 9999);
        cr_assert_fail("Should have thrown an exception");
    } catch (const AException& e) {
        // Vérifier que l'exception contient un message d'erreur approprié
        std::string message = e.getMessage();
        cr_assert(message.find("Connection failed") != std::string::npos ||
                 message.find("Connection timeout") != std::string::npos,
                 "Exception message should indicate connection failure");
    }
    
    cr_assert(!connection.isConnected(), "Connection should not be established after error");
}

Test(TcpConnection, NonBlockingIO, .init = redirect_all_std) {
    MockTcpServer server(4246);
    server.start();
    
    TcpConnection connection;
    connection.connect("127.0.0.1", 4246);
    
    // Attendre que la connexion soit acceptée
    bool connected = wait_for([&server]() { return server.clientSocket >= 0; });
    cr_assert(connected, "Server failed to accept client connection");
    
    // Vérifier que hasData() renvoie false quand il n'y a pas de données
    cr_assert(!connection.hasData(), "Should not have data initially");
    
    // Le serveur envoie un message au client
    server.sendData("test message\n");
    
    // Attendre et vérifier que hasData() renvoie true
    bool hasData = wait_for([&connection]() {
        return connection.hasData();
    });
    
    cr_assert(hasData, "Should detect available data after server sends message");
    
    connection.close();
    server.stop();
}
