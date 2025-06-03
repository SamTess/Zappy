/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour SystemWrapper
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/utils/SystemWrapper.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <chrono>
#include <thread>
#include <functional>

using namespace SystemWrapper;

// Tests pour SafeSockAddr
Test(SystemWrapper_SafeSockAddr, creation_manipulation) {
    // Créer une adresse sockaddr_in
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Construire SafeSockAddr à partir de sockaddr_in
    SafeSockAddr safeAddr(addr);
    
    // Vérifier les propriétés
    cr_assert_eq(safeAddr.getAddr().sin_family, AF_INET, "La famille d'adresse doit être AF_INET");
    cr_assert_eq(ntohs(safeAddr.getAddr().sin_port), 8080, "Le port doit être 8080");
    cr_assert_eq(safeAddr.getAddr().sin_addr.s_addr, inet_addr("127.0.0.1"), "L'adresse IP doit être 127.0.0.1");
    
    // Vérifier les méthodes d'accès
    cr_assert(safeAddr.getSockAddr() != nullptr, "getSockAddr() ne doit pas retourner nullptr");
    cr_assert_eq(safeAddr.getSize(), sizeof(sockaddr_in), "La taille doit correspondre à sizeof(sockaddr_in)");
    
    // Construction par défaut
    SafeSockAddr defaultAddr;
    cr_assert_eq(defaultAddr.getAddr().sin_family, 0, "La famille d'adresse par défaut doit être 0");
}

// Tests pour SafePollFd
Test(SystemWrapper_SafePollFd, creation_manipulation) {
    // Créer un SafePollFd avec un descripteur de fichier et des événements
    int fd = 5;  // Un descripteur de fichier factice
    SafePollFd pollFd(fd, POLLIN | POLLOUT);
    
    // Vérifier les propriétés
    cr_assert_eq(pollFd.getFd(), fd, "Le descripteur de fichier doit être 5");
    cr_assert_eq(pollFd.getEvents(), POLLIN | POLLOUT, "Les événements doivent être POLLIN | POLLOUT");
    cr_assert_eq(pollFd.getRevents(), 0, "Les revents doivent être initialisés à 0");
    
    // Modifier les propriétés
    pollFd.setFd(10);
    pollFd.setEvents(POLLIN);
    pollFd.setRevents(POLLERR);
    
    // Vérifier les changements
    cr_assert_eq(pollFd.getFd(), 10, "Le descripteur de fichier doit être modifié à 10");
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
