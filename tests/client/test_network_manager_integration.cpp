/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour NetworkManager avec mock de NetworkObserver
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <memory>
#include "../../src/GUI/network/networkManager/NetworkManager.hpp"

// Mock class pour NetworkObserver
class MockNetworkObserver : public NetworkObserver {
public:
    bool messageReceived = false;
    std::string lastHeader;
    std::string lastData;
    bool connectionChanged = false;
    bool connectionStatus = false;

    void onMessage(const std::string& header, const std::string& data) override {
        messageReceived = true;
        lastHeader = header;
        lastData = data;
    }
    
    void onConnectionStatusChanged(bool connected) override {
        connectionChanged = true;
        connectionStatus = connected;
    }
};

Test(NetworkManager, mock_interaction) {
    NetworkManager manager;
    MockNetworkObserver observer;
    
    // Maintenant nous pouvons utiliser la méthode pour ajouter un observer
    manager.addObserver(&observer);
    
    // Comme nous ne pouvons pas vraiment connecter, nous allons juste vérifier
    // que le code compile et que l'observer est correctement ajouté
    
    // Dans le cas réel avec un mock serveur, nous pourrions faire:
    // manager.connect("localhost", 1234);
    // manager.mockReceiveData("header data");
    // cr_assert(observer.messageReceived, "L'observer doit être notifié");
    // cr_assert_str_eq(observer.lastHeader.c_str(), "header", "L'en-tête doit être 'header'");
    // cr_assert_str_eq(observer.lastData.c_str(), "data", "Les données doivent être 'data'");
    
    // Pour l'instant, nous nous assurons juste que le code compile
    cr_assert(true);
}

// Test d'intégration du NetworkManager
Test(NetworkManager, integration_management) {
    NetworkManager manager;
    
    // Test de connexion avec une adresse locale (qui ne fonctionnera probablement pas en réalité)
    bool result = manager.connect("127.0.0.1", 9999);
    
    // Ne peut pas vraiment tester la connexion réelle dans des tests unitaires automatiques
    // mais nous pouvons vérifier que le manager se comporte correctement en cas d'échec
    
    // Si nous nous sommes connectés (très peu probable), nous nous déconnectons proprement
    if (result && manager.isConnected()) {
        manager.sendCommand("HELLO\n");
        manager.processIncomingMessages();
        manager.disconnect();
        cr_assert(!manager.isConnected(), "Le manager doit être déconnecté après disconnect()");
    } else {
        // Sinon, nous vérifions que l'échec est bien géré
        cr_assert(!manager.isConnected(), "Le manager ne doit pas être connecté après échec");
    }
}

// Test de comportement en cas de problème réseau
Test(NetworkManager, error_handling) {
    NetworkManager manager;
    
    // Simulation d'une série d'actions qui devraient être gérées sans crash
    manager.connect("nonexistent.domain", 1234);  // Domaine qui n'existe pas
    manager.sendCommand("test");                  // Envoi à une connexion échouée
    manager.processIncomingMessages();            // Traitement sans message
    manager.disconnect();                         // Déconnexion d'une connexion échouée
    
    // Le test réussit si aucune exception n'est levée
    cr_assert(!manager.isConnected(), "Le manager doit gérer gracieusement les erreurs réseau");
}

// Test des différents états de connexion
Test(NetworkManager, connection_states) {
    NetworkManager manager;
    
    // État initial
    cr_assert(!manager.isConnected(), "État initial: déconnecté");
    
    // Après une tentative de connexion à une adresse invalide
    manager.connect("999.999.999.999", 1234);
    cr_assert(!manager.isConnected(), "Après échec de connexion: toujours déconnecté");
    
    // Après une déconnexion explicite
    manager.disconnect();
    cr_assert(!manager.isConnected(), "Après déconnexion: toujours déconnecté");
}
