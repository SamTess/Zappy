/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour NetworkManager
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../src/GUI/network/networkManager/NetworkManager.hpp"
#include "../../src/GUI/network/connection/TcpConnection.hpp"
#include "../../src/GUI/network/networkThread/NetworkThread.hpp"
#include "../../src/GUI/network/buffer/MessageQueue.hpp"
#include "../../src/GUI/network/buffer/CircularBuffer.hpp"
#include <thread>
#include <chrono>

Test(NetworkManager, creation) {
    NetworkManager manager;
    cr_assert(!manager.isConnected(), "Le gestionnaire ne doit pas être connecté lors de la création");
}

Test(NetworkManager, connection_failure) {
    NetworkManager manager;
    
    // Tenter de se connecter à une adresse qui n'existe probablement pas
    bool result = manager.connect("256.256.256.256", 1234); // Adresse IPv4 invalide
    
    cr_assert(!result, "La connexion à une adresse IP invalide doit échouer");
    cr_assert(!manager.isConnected(), "Le gestionnaire ne doit pas être connecté après échec de connexion");
}

Test(NetworkManager, connect_disconnect) {
    NetworkManager manager;
    
    // Nous ne pouvons pas réellement nous connecter à un serveur dans un test unitaire,
    // mais nous pouvons vérifier que disconnect() ne cause pas de crash
    manager.disconnect();
    cr_assert(!manager.isConnected(), "Le gestionnaire doit être déconnecté après disconnect");
}

Test(NetworkManager, send_command_when_not_connected) {
    NetworkManager manager;
    
    // L'envoi d'une commande sans être connecté ne devrait pas crash
    manager.sendCommand("test");
    cr_assert(!manager.isConnected(), "Le statut de connexion ne doit pas changer après l'envoi d'une commande");
}

Test(NetworkManager, process_incoming_messages_when_not_connected) {
    NetworkManager manager;
    
    // Le traitement des messages sans être connecté ne devrait pas crash
    manager.processIncomingMessages();
    cr_assert(!manager.isConnected(), "Le statut de connexion ne doit pas changer après le traitement des messages");
}

// Note: Pour des tests plus complets, nous aurions besoin de mocker TcpConnection
// et d'autres dépendances pour simuler des connexions réussies et des échanges de données.
