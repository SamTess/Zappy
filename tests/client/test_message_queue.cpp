/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour MessageQueue
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../src/GUI/network/buffer/MessageQueue.hpp"

Test(MessageQueue, creation) {
    MessageQueue queue;
    cr_assert(queue.isEmpty(), "Nouvelle file doit être vide");
    cr_assert_eq(queue.size(), 0, "Nouvelle file doit avoir une taille de 0");
}

Test(MessageQueue, enqueue_dequeue) {
    MessageQueue queue;
    std::string message = "Test message";
    
    queue.enqueue(message);
    cr_assert(!queue.isEmpty(), "File ne doit pas être vide après ajout d'un message");
    cr_assert_eq(queue.size(), 1, "Taille de la file doit être 1");
    
    std::string received = queue.dequeue();
    cr_assert_str_eq(received.c_str(), message.c_str(), "Message défilé doit être identique au message enfilé");
    cr_assert(queue.isEmpty(), "File doit être vide après avoir défilé le seul message");
}

Test(MessageQueue, multiple_messages) {
    MessageQueue queue;
    
    queue.enqueue("First");
    queue.enqueue("Second");
    queue.enqueue("Third");
    
    cr_assert_eq(queue.size(), 3, "Taille de la file doit être 3");
    
    cr_assert_str_eq(queue.dequeue().c_str(), "First", "Premier message défilé doit être 'First'");
    cr_assert_str_eq(queue.dequeue().c_str(), "Second", "Deuxième message défilé doit être 'Second'");
    cr_assert_str_eq(queue.dequeue().c_str(), "Third", "Troisième message défilé doit être 'Third'");
    
    cr_assert(queue.isEmpty(), "File doit être vide après avoir défilé tous les messages");
}

Test(MessageQueue, dequeue_empty) {
    MessageQueue queue;
    std::string received = queue.dequeue();
    
    cr_assert_str_eq(received.c_str(), "", "Défilage d'une file vide doit retourner une chaîne vide");
    cr_assert(queue.isEmpty(), "File doit rester vide");
}

Test(MessageQueue, clear) {
    MessageQueue queue;
    
    queue.enqueue("Message 1");
    queue.enqueue("Message 2");
    queue.enqueue("Message 3");
    
    cr_assert(!queue.isEmpty(), "File ne doit pas être vide après ajout de messages");
    
    queue.clear();
    cr_assert(queue.isEmpty(), "File doit être vide après clear");
    cr_assert_eq(queue.size(), 0, "Taille de la file doit être 0 après clear");
}
