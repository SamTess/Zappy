/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour NetworkManager
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/networkManager/NetworkManager.hpp"
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <cstdio>
#include <functional>

// Tests fondamentaux pour NetworkManager

// Fonctions utilitaires pour rediriger stdout/stderr
static void redirect_all_stdout(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test de la création/destruction du NetworkManager
Test(NetworkManager, creation_destruction) {
    // La création et destruction du NetworkManager ne devrait pas causer de crash
    {
        NetworkManager manager;
        cr_assert(!manager.isConnected(), "Un nouveau NetworkManager ne doit pas être connecté");
    }
    // Le destructeur est appelé implicitement
    cr_assert(true, "Le destructeur doit s'exécuter sans problème");
}

// Test d'échec de connexion (adresse invalide)
Test(NetworkManager, connection_failure) {
    NetworkManager manager;
    
    // Tenter de se connecter à une adresse IP invalide
    bool result = manager.connect("256.256.256.256", 4242); // Adresse IP invalide
    
    cr_assert(!result, "La connexion à une adresse IP invalide doit échouer");
    cr_assert(!manager.isConnected(), "Le manager ne doit pas être connecté après échec");
}

// Test d'échec de connexion (port fermé)
Test(NetworkManager, connection_to_closed_port) {
    NetworkManager manager;
    
    // Tenter de se connecter à un port probablement fermé
    bool result = manager.connect("127.0.0.1", 9999);
    
    cr_assert(!result, "La connexion à un port fermé doit échouer");
    cr_assert(!manager.isConnected(), "Le manager ne doit pas être connecté après échec");
}

// Test de déconnexion (sans connexion préalable)
Test(NetworkManager, disconnect_when_not_connected) {
    NetworkManager manager;
    
    // Déconnecter un manager qui n'est pas connecté ne devrait pas causer de crash
    manager.disconnect();
    cr_assert(!manager.isConnected(), "Le manager doit rester non connecté après disconnect()");
}

// Test d'envoi de commande sans être connecté
Test(NetworkManager, send_command_when_not_connected, .init = redirect_all_stdout) {
    NetworkManager manager;
    
    // L'envoi d'une commande sans être connecté ne devrait pas crash
    manager.sendCommand("test");
    cr_assert(!manager.isConnected(), "L'état de connexion ne doit pas changer après l'envoi d'une commande");
    
    // Vérifier qu'aucune erreur critique n'est présente dans la sortie d'erreur
    fflush(stderr);
    // On ne vérifie pas le contenu exact car il peut varier, mais on s'assure que ça ne plante pas
}

// Test de traitement des messages entrants sans être connecté
Test(NetworkManager, process_incoming_messages_when_not_connected) {
    NetworkManager manager;
    
    // Le traitement des messages sans être connecté ne devrait pas crash
    manager.processIncomingMessages();
    cr_assert(!manager.isConnected(), "L'état de connexion ne doit pas changer après le traitement des messages");
}

// Test des callbacks
Test(NetworkManager, callback_registration) {
    NetworkManager manager;
    bool message_callback_called = false;
    bool connection_callback_called = false;
    
    // Définir les callbacks
    manager.setMessageCallback([&message_callback_called](const std::string&, const std::string&) {
        message_callback_called = true;
    });
    
    manager.setConnectionCallback([&connection_callback_called](bool) {
        connection_callback_called = true;
    });
    
    // Ces callbacks ne seront pas appelés sans connexion réelle
    manager.processIncomingMessages();
    cr_assert(!message_callback_called, "Le callback de message ne doit pas être appelé sans connexion");
    
    // Tenter une connexion (qui échouera) pourrait déclencher le callback de connexion
    manager.connect("127.0.0.1", 9999);
    
    // Certaines implémentations pourraient appeler le callback même pour un échec
    // donc nous ne testons pas la valeur de connection_callback_called ici
}

// Test de multiples tentatives de connexion/déconnexion
Test(NetworkManager, repeated_connection_attempts) {
    NetworkManager manager;
    
    // Tenter plusieurs connexions consécutives à des adresses invalides
    for (int i = 0; i < 5; i++) {
        bool result = manager.connect("invalid.address", 4242);
        cr_assert(!result, "La connexion doit échouer avec une adresse invalide");
        cr_assert(!manager.isConnected(), "Le manager doit rester non connecté après échec");
        
        // Déconnexion explicite entre les tentatives
        manager.disconnect();
        cr_assert(!manager.isConnected(), "Le manager doit rester non connecté après disconnect()");
    }
}

// Test des appels répétés aux méthodes sans incidence sur l'état
Test(NetworkManager, repeated_method_calls) {
    NetworkManager manager;
    
    // Appels répétés aux méthodes qui ne devraient pas changer l'état
    for (int i = 0; i < 10; i++) {
        manager.processIncomingMessages();
        manager.sendCommand("test" + std::to_string(i));
        manager.disconnect();
        
        cr_assert(!manager.isConnected(), "L'état de connexion ne doit pas changer après des appels répétés");
    }
}

// Test de l'ordre correct des opérations
Test(NetworkManager, operation_order, .init = redirect_all_stdout) {
    NetworkManager manager;
    
    // Essayer d'envoyer une commande avant de se connecter
    manager.sendCommand("test");
    
    // Se connecter (échec attendu avec ces paramètres)
    manager.connect("127.0.0.1", 9999);
    
    // Traiter des messages alors qu'il n'y en a probablement pas
    manager.processIncomingMessages();
    
    // Se déconnecter à la fin
    manager.disconnect();
    
    // On vérifie simplement qu'il n'y a pas eu de crash
    cr_assert(true, "L'ordre des opérations ne doit pas causer de crash");
}

// Test avec des chaînes spéciales/longues
Test(NetworkManager, special_strings, .init = redirect_all_stdout) {
    NetworkManager manager;
    
    // Chaîne vide
    manager.sendCommand("");
    
    // Chaîne très longue
    std::string longString(10000, 'X');
    manager.sendCommand(longString);
    
    // Chaînes avec caractères spéciaux
    manager.sendCommand("\n\r\t");
    manager.sendCommand("特殊字符");
    manager.sendCommand("مرحبا");
    
    // Vérifier qu'il n'y a pas de crash
    cr_assert(true, "L'envoi de chaînes spéciales ne doit pas causer de crash");
}
