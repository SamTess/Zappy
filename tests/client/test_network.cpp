/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests pour les composants réseau du client
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
#include "../../src/GUI/network/buffer/CircularBuffer.hpp"
#include "../../src/GUI/network/buffer/MessageQueue.hpp"
#include "../../src/GUI/network/connection/TcpConnection.hpp"
#include "../../src/GUI/network/networkManager/NetworkLogger.hpp"
#include "../../src/GUI/shared/exception/AException.hpp"

// Structure de serveur mock pour les tests
struct MockServer {
    int serverSocket;
    int clientSocket;
    struct sockaddr_in address;
    bool isRunning;
    std::thread serverThread;

    MockServer(int port = 4242) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        cr_assert(serverSocket > 0, "Failed to create mock server socket");

        int opt = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        cr_assert(bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) >= 0,
                "Failed to bind mock server socket");
        cr_assert(listen(serverSocket, 3) >= 0, "Failed to listen on mock server socket");
        
        clientSocket = -1;
        isRunning = false;
    }

    void start() {
        isRunning = true;
        serverThread = std::thread([this]() {
            socklen_t addrlen = sizeof(address);
            clientSocket = accept(serverSocket, (struct sockaddr *)&address, &addrlen);
            
            if (clientSocket < 0) {
                isRunning = false;
                return;
            }

            // Buffer pour recevoir les données du client
            char buffer[1024] = {0};
            int valread = read(clientSocket, buffer, 1024);
            if (valread > 0) {
                cr_log_info("Server received: %s", buffer);
                // Répond avec "msz 10 10\n" pour simuler une réponse du serveur
                const char *hello = "msz 10 10\n";
                send(clientSocket, hello, strlen(hello), 0);
            }
            
            while (isRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    ~MockServer() {
        stop();
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
};

// Tests pour CircularBuffer
Test(circular_buffer, initialization) {
    CircularBuffer buffer(100);
    cr_assert(buffer.isEmpty(), "New buffer should be empty");
    cr_assert_eq(buffer.available(), 0, "New buffer should have 0 available bytes");
    cr_assert_not(buffer.isFull(), "New buffer should not be full");
}

Test(circular_buffer, write_and_read) {
    CircularBuffer buffer(10);
    const char *data = "Hello";
    buffer.write(data, 5);
    
    cr_assert_eq(buffer.available(), 5, "Buffer should have 5 bytes available after write");
    cr_assert_not(buffer.isEmpty(), "Buffer shouldn't be empty after write");
    
    char readBuffer[10] = {0};
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, 5, "Should read 5 bytes from buffer");
    cr_assert_str_eq(readBuffer, "Hello", "Read data should match written data");
    cr_assert(buffer.isEmpty(), "Buffer should be empty after read");
}

Test(circular_buffer, auto_resize) {
    CircularBuffer buffer(5);
    const char *data = "Hello, world!";
    buffer.write(data, 13);
    
    cr_assert_eq(buffer.available(), 13, "Buffer should resize and contain all data");
    
    char readBuffer[20] = {0};
    size_t bytesRead = buffer.read(readBuffer, 20);
    
    cr_assert_eq(bytesRead, 13, "Should read all 13 bytes from buffer");
    cr_assert_str_eq(readBuffer, "Hello, world!", "Read data should match written data");
}

Test(circular_buffer, wrap_around) {
    CircularBuffer buffer(10);
    const char *data1 = "12345";
    const char *data2 = "67890";
    
    buffer.write(data1, 5);
    char readBuffer[10] = {0};
    buffer.read(readBuffer, 3); // Read 3 bytes, leaving 2 in buffer
    
    buffer.write(data2, 5);
    memset(readBuffer, 0, sizeof(readBuffer));
    size_t bytesRead = buffer.read(readBuffer, 10);
    
    cr_assert_eq(bytesRead, 7, "Should read 7 bytes (2 remaining + 5 new)");
    cr_assert_str_eq(readBuffer, "4567890", "Read data should match expected data");
}

// Tests pour MessageQueue
Test(message_queue, initialization) {
    MessageQueue queue;
    cr_assert(queue.isEmpty(), "New queue should be empty");
    cr_assert_eq(queue.size(), 0, "New queue should have size 0");
}

Test(message_queue, enqueue_dequeue) {
    MessageQueue queue;
    queue.enqueue("Hello");
    queue.enqueue("World");
    
    cr_assert_eq(queue.size(), 2, "Queue should have 2 items");
    cr_assert_not(queue.isEmpty(), "Queue shouldn't be empty");
    
    std::string message1 = queue.dequeue();
    std::string message2 = queue.dequeue();
    
    cr_assert_str_eq(message1.c_str(), "Hello", "First message should be 'Hello'");
    cr_assert_str_eq(message2.c_str(), "World", "Second message should be 'World'");
    cr_assert(queue.isEmpty(), "Queue should be empty after dequeuing all items");
}

Test(message_queue, dequeue_empty) {
    MessageQueue queue;
    std::string message = queue.dequeue();
    
    cr_assert_str_eq(message.c_str(), "", "Dequeue on empty queue should return empty string");
}

Test(message_queue, clear) {
    MessageQueue queue;
    queue.enqueue("Hello");
    queue.enqueue("World");
    
    queue.clear();
    cr_assert(queue.isEmpty(), "Queue should be empty after clear");
    cr_assert_eq(queue.size(), 0, "Queue size should be 0 after clear");
}

// Fixture pour rediriger stderr
void redirect_stderr(void) {
    cr_redirect_stderr();
}

// Tests pour TcpConnection
Test(tcp_connection, initialization) {
    TcpConnection connection;
    cr_assert_not(connection.isConnected(), "New connection should not be connected");
    cr_assert_not(connection.hasData(), "New connection should not have data");
}

// Ce test nécessite un serveur mock
Test(tcp_connection, connect_and_send, .init=redirect_stderr) {
    MockServer server(4243);
    server.start();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre que le serveur démarre
    
    TcpConnection connection;
    connection.connect("127.0.0.1", 4243);
    
    cr_assert(connection.isConnected(), "Connection should be established");
    
    connection.send("GRAPHIC\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre la réponse
    
    cr_assert(connection.hasData(), "Connection should have data after server response");
    
    std::string response = connection.receive();
    cr_assert_str_eq(response.c_str(), "msz 10 10\n", "Response should match expected server data");
    
    connection.close();
    cr_assert_not(connection.isConnected(), "Connection should be closed");
}

// Test pour gérer les erreurs de connexion
Test(tcp_connection, connect_error, .init=redirect_stderr) {
    TcpConnection connection;
    
    // Tenter de se connecter à un port où aucun serveur n'écoute
    bool exceptionThrown = false;
    try {
        connection.connect("127.0.0.1", 65432); // Port probablement inutilisé
    } catch (const AException &e) {
        exceptionThrown = true;
    }
    
    cr_assert(exceptionThrown, "Exception should be thrown for connection to invalid port");
    cr_assert_not(connection.isConnected(), "Connection should not be established");
}

// Test de Network Logger
Test(network_logger, logging) {
    NetworkLogger::get().setEnabled(true);
    cr_assert(NetworkLogger::get().isEnabled(), "Logger should be enabled");

    NetworkLogger::get().setEnabled(false);
    cr_assert_not(NetworkLogger::get().isEnabled(), "Logger should be disabled");
}
