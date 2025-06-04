/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour TcpConnection
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/connection/TcpConnection.hpp"
#include <thread>
#include <chrono>
#include <future>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>

// Port utilisé pour les tests (port non privilégié > 1024)
#define TEST_PORT 8765

// Fonction pour créer un serveur simple qui accepte une connexion
void setup_test_server(int port, std::promise<bool> &server_ready) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        server_ready.set_value(false);
        return;
    }
    
    // Permettre la réutilisation de l'adresse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        close(server_fd);
        server_ready.set_value(false);
        return;
    }
    
    if (listen(server_fd, 3) < 0) {
        close(server_fd);
        server_ready.set_value(false);
        return;
    }
    
    // Le serveur est prêt à accepter des connexions
    server_ready.set_value(true);
    
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    
    // Accepter une connexion client
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
    
    // Écrire des données de test
    if (client_fd >= 0) {
        const char *message = "TEST_MESSAGE\n";
        send(client_fd, message, strlen(message), 0);
        
        // Attendre et lire une réponse
        char buffer[128] = {0};
        recv(client_fd, buffer, sizeof(buffer), 0);
        
        // Fermer la connexion client
        close(client_fd);
    }
    
    close(server_fd);
}

// Tests pour la création d'une connexion TCP
Test(TcpConnection, creation_destruction) {
    TcpConnection connection;
    cr_assert(!connection.isConnected(), "Une nouvelle connexion ne doit pas être connectée");
}

// Test de connexion à un serveur inexistant
Test(TcpConnection, connect_to_nonexistent_server) {
    TcpConnection connection;
    
    // Tenter de se connecter à un hôte inexistant
    bool exception_thrown = false;
    try {
        connection.connect("nonexistent.host.invalid", TEST_PORT);
    } catch (const std::exception &) {
        exception_thrown = true;
    }
    
    cr_assert(exception_thrown, "Une exception doit être levée lors d'une connexion à un hôte inexistant");
    cr_assert(!connection.isConnected(), "La connexion doit être dans un état non connecté après échec");
}

// Test de connexion à un serveur valide
Test(TcpConnection, connect_to_valid_server) {
    // Lancer un serveur de test dans un thread séparé
    std::promise<bool> server_ready_promise;
    std::future<bool> server_ready_future = server_ready_promise.get_future();
    
    std::thread server_thread(setup_test_server, TEST_PORT, std::ref(server_ready_promise));
    server_thread.detach();
    
    // Attendre que le serveur soit prêt
    bool is_server_ready = false;
    try {
        // std::future::wait_for retourne une enum qui indique si le futur est prêt
        auto status = server_ready_future.wait_for(std::chrono::seconds(2));
        if (status == std::future_status::ready) {
            is_server_ready = server_ready_future.get();
        } else {
            cr_assert(false, "Timeout en attendant le démarrage du serveur de test");
        }
    } catch (const std::future_error &) {
        cr_assert(false, "Le serveur de test n'a pas démarré correctement");
    }
    
    if (!is_server_ready) {
        cr_assert(false, "Le serveur de test n'a pas pu être initialisé");
        return;
    }
    
    // Tenter de se connecter au serveur
    TcpConnection connection;
    bool connected = false;
    
    try {
        connection.connect("127.0.0.1", TEST_PORT);
        connected = true;
    } catch (const std::exception &e) {
        cr_log_error("Exception lors de la connexion: %s", e.what());
    }
    
    cr_assert(connected, "La connexion doit réussir avec un serveur valide");
    cr_assert(connection.isConnected(), "La connexion doit être dans un état connecté après succès");
    
    // Attendre un peu pour recevoir des données
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Vérifier la réception des données
    std::string received = connection.receive();
    cr_assert_str_eq(received.c_str(), "TEST_MESSAGE\n", "Les données reçues doivent correspondre à celles envoyées");
    
    // Envoyer une réponse
    connection.send("RESPONSE\n");
    
    // Fermer la connexion
    connection.close();
    cr_assert(!connection.isConnected(), "La connexion doit être fermée après close()");
}

// Test de l'échec de connexion à un port fermé
Test(TcpConnection, connect_to_closed_port) {
    TcpConnection connection;
    
    // Tenter de se connecter à un port probablement fermé
    bool exception_thrown = false;
    try {
        connection.connect("127.0.0.1", 9999);
    } catch (const std::exception &) {
        exception_thrown = true;
    }
    
    cr_assert(exception_thrown, "Une exception doit être levée lors d'une connexion à un port fermé");
    cr_assert(!connection.isConnected(), "La connexion doit être dans un état non connecté après échec");
}

// Test de fermeture d'une connexion non établie
Test(TcpConnection, close_nonconnected) {
    TcpConnection connection;
    
    // Fermer une connexion qui n'a jamais été établie ne devrait pas causer d'erreur
    connection.close();
    cr_assert(!connection.isConnected(), "La connexion doit rester non connectée après close()");
}

// Test des méthodes d'état de la connexion
Test(TcpConnection, connection_state) {
    TcpConnection connection;
    
    // État initial
    cr_assert(!connection.isConnected(), "Une nouvelle connexion doit être non connectée");
    cr_assert(connection.hasData() == true, "hasData() doit toujours renvoyer true pour vérifier régulièrement le socket");
    
    // Après une tentative de connexion échouée
    try {
        connection.connect("127.0.0.1", 9999);
    } catch (const std::exception &) {
        // Ignorer l'exception, c'est attendu
    }
    
    cr_assert(!connection.isConnected(), "La connexion doit être non connectée après échec");
    cr_assert(connection.hasData() == true, "hasData() doit toujours renvoyer true pour vérifier régulièrement le socket");
    
    // Après fermeture explicite
    connection.close();
    cr_assert(!connection.isConnected(), "La connexion doit être non connectée après close()");
}
