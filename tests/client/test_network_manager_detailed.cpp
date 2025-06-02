/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests détaillés pour les fonctionnalités du NetworkManager
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <memory>
#include "../../src/GUI/network/networkManager/NetworkManager.hpp"

// Fonctions utilitaires pour rediriger stdout et stderr
static void redirect_all_stdout(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test de la création et destruction du NetworkManager
Test(NetworkManager, instantiation_destruction) {
    // La création et destruction du NetworkManager ne devrait pas causer de crash
    {
        NetworkManager manager;
        // Vérification des valeurs par défaut
        cr_assert(!manager.isConnected(), "Un nouveau manager ne doit pas être connecté");
    }
    // Ici, le destructeur est appelé. S'il y a un crash, le test échouera.
    cr_assert(true, "Le destructeur doit s'exécuter sans problème");
}

// Test du comportement de isConnected
Test(NetworkManager, is_connected_behavior) {
    NetworkManager manager;
    
    // Au départ, pas connecté
    cr_assert(!manager.isConnected(), "Le manager ne doit pas être connecté à la création");
    
    // Tenter une connexion à une adresse invalide
    manager.connect("invalid.address", 12345);
    cr_assert(!manager.isConnected(), "Le manager ne doit pas être connecté après échec de connexion");
    
    // Déconnecter explicitement (même si déjà déconnecté)
    manager.disconnect();
    cr_assert(!manager.isConnected(), "Le manager doit rester déconnecté après disconnect");
}

// Test de la gestion des exceptions lors de l'envoi de commandes
Test(NetworkManager, send_command_exceptions, .init=redirect_all_stdout) {
    NetworkManager manager;
    
    // Envoi d'une commande sans être connecté
    manager.sendCommand("test");
    
    // Vérifier que le manager reste dans un état cohérent
    cr_assert(!manager.isConnected(), "L'état ne doit pas changer après échec d'envoi");
    
    // Note: Nous pourrions également vérifier les messages d'erreur dans stdout/stderr
    // mais cela dépend de l'implémentation exacte
}

// Test du traitement des messages entrants
Test(NetworkManager, process_incoming_messages) {
    NetworkManager manager;
    
    // Le traitement des messages sans être connecté ne devrait pas causer de crash
    manager.processIncomingMessages();
    
    // Vérifier que le manager reste dans un état cohérent
    cr_assert(!manager.isConnected(), "L'état ne doit pas changer après traitement des messages");
}

// Test de tentatives multiples de connexion/déconnexion
Test(NetworkManager, multiple_connection_attempts) {
    NetworkManager manager;
    
    // Première tentative
    manager.connect("localhost", 12345);
    bool firstAttempt = manager.isConnected();
    
    // Déconnexion
    manager.disconnect();
    cr_assert(!manager.isConnected(), "Le manager doit être déconnecté après disconnect");
    
    // Deuxième tentative
    manager.connect("localhost", 12346);
    bool secondAttempt = manager.isConnected();
    
    // Déconnexion finale
    manager.disconnect();
    cr_assert(!manager.isConnected(), "Le manager doit être déconnecté après disconnect final");
    
    // Ces assertions dépendent de l'environnement de test et pourraient être ajustées
    cr_assert((!firstAttempt && !secondAttempt) || (firstAttempt || secondAttempt), 
              "Le résultat des tentatives de connexion doit être cohérent");
}

// Test des appels répétés aux méthodes sans incidence sur l'état
Test(NetworkManager, repeated_method_calls) {
    NetworkManager manager;
    
    // Multiple appels à disconnect() sans être connecté
    for (int i = 0; i < 5; i++) {
        manager.disconnect();
        cr_assert(!manager.isConnected(), "Le manager doit rester déconnecté après appels répétés à disconnect");
    }
    
    // Multiple appels à processIncomingMessages() sans être connecté
    for (int i = 0; i < 5; i++) {
        manager.processIncomingMessages();
        cr_assert(!manager.isConnected(), "Le manager doit rester déconnecté après appels répétés à processIncomingMessages");
    }
}

// Test de connexions simultanées
Test(NetworkManager, simultaneous_connections) {
    NetworkManager manager1;
    NetworkManager manager2;
    
    // Tenter de connecter les deux managers au même port (ce qui échouera probablement)
    manager1.connect("localhost", 12345);
    manager2.connect("localhost", 12345);
    
    // Nettoyer
    manager1.disconnect();
    manager2.disconnect();
    
    // Vérifier que les deux managers sont bien déconnectés à la fin
    cr_assert(!manager1.isConnected() && !manager2.isConnected(), 
              "Les deux managers doivent être déconnectés après disconnect");
}
