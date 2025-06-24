/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour NetworkAPI
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/systemWrapper/NetworkAPI.hpp"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <functional>

using namespace Network;

// Tests pour NetworkAPI - Factory
Test(NetworkAPI_Factory, create_factory) {
    auto factory = createFactory();
    cr_assert(factory != nullptr, "createFactory() ne doit pas retourner nullptr");
}

// Tests pour NetworkAPI - Socket creation
Test(NetworkAPI_Socket, create_tcp_socket) {
    auto factory = createFactory();
    auto socket = factory->createTcpSocket();
    cr_assert(socket != nullptr, "createTcpSocket() ne doit pas retourner nullptr");
    cr_assert(!socket->isConnected(), "Un nouveau socket ne doit pas être connecté");
}

// Tests pour NetworkAPI - Buffer creation
Test(NetworkAPI_Buffer, create_buffer) {
    auto factory = createFactory();
    auto buffer = factory->createBuffer(1024);
    cr_assert(buffer != nullptr, "createBuffer() ne doit pas retourner nullptr");
    cr_assert_eq(buffer->size(), 1024, "La taille du buffer doit être 1024");
    
    // Test de manipulation des données
    buffer->setData("Hello World");
    cr_assert_str_eq(buffer->getData().c_str(), "Hello World", "Les données doivent être 'Hello World'");
    cr_assert_str_eq(buffer->toString().c_str(), "Hello World", "toString() doit retourner 'Hello World'");
}

// Tests pour NetworkAPI - Address creation
Test(NetworkAPI_Address, create_address) {
    auto factory = createFactory();
    auto address = factory->createAddress("localhost", 8080);
    cr_assert(address != nullptr, "createAddress() ne doit pas retourner nullptr");
    cr_assert_str_eq(address->getHost().c_str(), "localhost", "L'hôte doit être 'localhost'");
    cr_assert_eq(address->getPort(), 8080, "Le port doit être 8080");
    cr_assert_neq(address->toString().size(), 0, "toString() ne doit pas être vide");
}

// Tests pour NetworkAPI - Poller creation
Test(NetworkAPI_Poller, create_poller) {
    auto factory = createFactory();
    auto poller = factory->createPoller();
    cr_assert(poller != nullptr, "createPoller() ne doit pas retourner nullptr");
    
    // Test de manipulation basique
    poller->clear();
    int result = poller->poll(100);  // timeout de 100ms
    cr_assert_eq(result, 0, "poll() sans socket doit retourner 0 (timeout)");
}
    cr_assert_eq(pollFd.getEvents(), POLLIN, "Les événements doivent être modifiés à POLLIN");
    cr_assert_eq(pollFd.getRevents(), POLLERR, "Les revents doivent être modifiés à POLLERR");
    
    // Vérifier les méthodes d'accès
    cr_assert(pollFd.getPollPtr() != nullptr, "getPollPtr() ne doit pas retourner nullptr");
}

// Tests pour SafeBuffer
Test(SystemWrapper_SafeBuffer, creation_manipulation) {
    // Créer un buffer de taille donnée
    SafeBuffer buffer(100);
    
    // Vérifier la taille
    cr_assert_eq(buffer.size(), 100, "La taille du buffer doit être 100");
    
    // Manipuler les données
    std::string& data = buffer.data();
    data = "Test string";
    
    // Vérifier les données
    cr_assert_eq(buffer.toString(), "Test string", "Le contenu du buffer doit être 'Test string'");
    
    // Redimensionner
    buffer.resize(50);
    cr_assert_eq(buffer.size(), 50, "La taille du buffer doit être 50 après redimensionnement");
}

// Tests pour les fonctions de création et manipulation de socket
Test(SystemWrapper_Socket, create_socket) {
    // Créer un socket
    int sock = createSocket(AF_INET, SOCK_STREAM, 0);
    cr_assert(sock >= 0, "La création de socket doit réussir");
    
    // Fermer le socket
    int closeResult = closeSocket(sock);
    cr_assert_eq(closeResult, 0, "La fermeture du socket doit réussir");
}

// Tests pour les fonctions de conversion d'adresses
Test(SystemWrapper_AddressConversion, inet_pton_ntop) {
    struct in_addr addr;
    
    // Convertir une chaîne en adresse binaire
    bool conversionSuccess = inetPton(AF_INET, "192.168.1.1", &addr);
    cr_assert(conversionSuccess, "La conversion de chaîne en adresse doit réussir");
    
    // Convertir de binaire à chaîne
    std::string addrStr = inetNtop(AF_INET, &addr);
    cr_assert_eq(addrStr, "192.168.1.1", "La conversion d'adresse en chaîne doit donner '192.168.1.1'");
}

// Tests pour executeWithTimeout
Test(SystemWrapper_Timeout, execute_with_timeout_success) {
    bool actionExecuted = false;
    
    // Exécuter une action rapide qui devrait se terminer avant le timeout
    executeWithTimeout([&actionExecuted]() {
        actionExecuted = true;
        return true;  // L'action est terminée
    }, 1000);
    
    cr_assert(actionExecuted, "L'action doit être exécutée avant le timeout");
}

Test(SystemWrapper_Timeout, execute_with_timeout_timeout) {
    bool actionCompleted = false;
    
    // Exécuter une action qui ne se termine jamais
    executeWithTimeout([&actionCompleted]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return false;  // L'action n'est pas terminée
    }, 100);  // Timeout court
    
    cr_assert(!actionCompleted, "L'action ne doit pas être considérée comme terminée après un timeout");
}

// Test pour les fonctions de manipulation de tableaux
Test(SystemWrapper_Arrays, make_unique_shared_arrays) {
    // Créer un tableau unique_ptr
    auto uniqueArray = makeUniqueArray<int>(10);
    cr_assert(uniqueArray != nullptr, "makeUniqueArray doit créer un tableau valide");
    
    // Remplir quelques valeurs
    for (int i = 0; i < 10; i++) {
        uniqueArray[i] = i * 2;
    }
    
    // Vérifier les valeurs
    cr_assert_eq(uniqueArray[5], 10, "La valeur à l'index 5 doit être 10");
    
    // Créer un tableau shared_ptr
    auto sharedArray = makeSharedArray<char>(20);
    cr_assert(sharedArray != nullptr, "makeSharedArray doit créer un tableau valide");
    
    // Remplir quelques valeurs
    for (int i = 0; i < 20; i++) {
        sharedArray[i] = 'A' + i;
    }
    
    // Vérifier les valeurs
    cr_assert_eq(sharedArray[0], 'A', "La valeur à l'index 0 doit être 'A'");
    cr_assert_eq(sharedArray[5], 'F', "La valeur à l'index 5 doit être 'F'");
}
