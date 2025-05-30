/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour TcpConnection
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include "../../src/GUI/network/connection/TcpConnection.hpp"

// Ce test nécessite généralement un serveur mock pour fonctionner correctement,
// mais nous allons utiliser une approche simplifiée pour les tests de base.

// Pour un test complet, nous aurions besoin de configurer un serveur TCP fictif
// Pour l'instant, nous allons principalement tester la création et les cas d'erreur

Test(TcpConnection, creation) {
    TcpConnection connection;
    cr_assert(!connection.isConnected(), "Une nouvelle connexion ne doit pas être connectée");
    cr_assert(!connection.hasData(), "Une nouvelle connexion ne doit pas avoir de données");
}

Test(TcpConnection, connection_failure) {
    TcpConnection connection;
    
    // Tenter de se connecter à une adresse qui n'existe probablement pas
    // Cela devrait échouer et ne pas planter
    try {
        connection.connect("256.256.256.256", 1234); // Adresse IPv4 invalide
        cr_assert_fail("La connexion à une adresse IP invalide devrait échouer");
    } catch (const std::exception& e) {
        cr_assert_str_neq(e.what(), "", "L'exception doit avoir un message");
    }
    
    cr_assert(!connection.isConnected(), "La connexion ne doit pas être établie après échec");
}

// Note: Pour les tests de connexion réussie, envoi et réception,
// il faudrait idéalement configurer un serveur de test local.

// Test simulé pour vérifier l'API
Test(TcpConnection, api_test) {
    TcpConnection connection;
    
    // Test simple des méthodes sans connexion réelle
    connection.close();
    cr_assert(!connection.isConnected(), "La connexion doit rester déconnectée après close");
    
    // Ces appels devraient échouer proprement sans crash
    try {
        connection.send("test");
    } catch (const std::exception&) {
        // Exception attendue
    }
    
    try {
        connection.receive();
    } catch (const std::exception&) {
        // Exception attendue
    }
}
