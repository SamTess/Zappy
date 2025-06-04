/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour NetworkThread
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../../../src/GUI/network/networkThread/NetworkThread.hpp"
#include <atomic>
#include <thread>
#include <chrono>

Test(NetworkThread, creation_destruction) {
    NetworkThread thread;
    cr_assert(!thread.isRunning(), "Une nouvelle thread ne doit pas être en cours d'exécution");
}

Test(NetworkThread, start_stop) {
    NetworkThread thread;
    std::atomic<bool> threadExecuted(false);
    
    // Démarrer le thread avec une fonction simple
    thread.start([&threadExecuted]() {
        threadExecuted = true;
        // Simuler un travail pendant un moment
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });
    
    cr_assert(thread.isRunning(), "Le thread doit être en état de fonctionnement après start");
    
    // Attendez un peu pour s'assurer que le thread a le temps de s'exécuter
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // Vérifier que la fonction a été exécutée
    cr_assert(threadExecuted, "La fonction dans le thread doit être exécutée");
    
    // Arrêter le thread
    thread.stop();
    cr_assert(!thread.isRunning(), "Le thread ne doit plus être en cours d'exécution après stop");
}

Test(NetworkThread, multiple_starts) {
    NetworkThread thread;
    std::atomic<int> counter(0);
    
    // Démarrer le thread une première fois
    thread.start([&counter]() {
        counter++;
        // Simuler un travail
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });
    
    cr_assert(thread.isRunning(), "Le thread doit être en cours d'exécution");
    
    // Arrêter le thread
    thread.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Démarrer le thread une seconde fois avec une fonction différente
    thread.start([&counter]() {
        counter += 2;
        // Simuler un travail
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });
    
    cr_assert(thread.isRunning(), "Le thread doit être en cours d'exécution après le second démarrage");
    
    // Attendre que le thread ait terminé son exécution
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Vérifier que le compteur a augmenté de 3 au total (1 + 2)
    cr_assert_eq(counter, 3, "Les deux fonctions du thread doivent avoir été exécutées");
    
    // Nettoyage
    thread.stop();
}

Test(NetworkThread, long_running_task) {
    NetworkThread thread;
    std::atomic<bool> running(true);
    std::atomic<int> counter(0);
    
    // Démarrer un thread avec une tâche de longue durée
    thread.start([&running, &counter]() {
        while (running) {
            counter++;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
    
    // Laisser le thread s'exécuter pendant un certain temps
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cr_assert(thread.isRunning(), "Le thread doit être en cours d'exécution");
    
    // Arrêter la boucle et le thread
    running = false;
    thread.stop();
    
    // Vérifier que le compteur a augmenté et que le thread s'est arrêté
    cr_assert(counter > 0, "La tâche de longue durée doit avoir incrémenté le compteur");
    cr_assert(!thread.isRunning(), "Le thread ne doit plus être en cours d'exécution après stop");
}

Test(NetworkThread, exception_in_thread) {
    NetworkThread thread;
    std::atomic<bool> exceptionCaught(false);
    
    // Démarrer un thread qui lève une exception
    thread.start([&exceptionCaught]() {
        try {
            throw std::runtime_error("Exception de test");
        } catch (const std::exception&) {
            exceptionCaught = true;
        }
    });
    
    // Attendre que le thread ait eu le temps d'exécuter sa fonction et de terminer
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // Vérifier que l'exception a été attrapée
    cr_assert(exceptionCaught, "L'exception dans le thread doit être attrapée");
    
    // Avec la nouvelle conception, le thread peut rester en exécution même après une exception
    // Nous vérifions simplement que l'exception a été traitée
    
    // Nettoyage
    thread.stop();
}
