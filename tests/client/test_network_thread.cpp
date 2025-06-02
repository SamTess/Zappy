/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour NetworkThread
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../src/GUI/network/networkThread/NetworkThread.hpp"
#include <atomic>
#include <chrono>
#include <thread>

Test(NetworkThread, creation) {
    NetworkThread thread;
    cr_assert(!thread.isRunning(), "Le thread ne doit pas être en cours d'exécution lors de la création");
}

Test(NetworkThread, start_stop) {
    NetworkThread thread;
    std::atomic<bool> threadDidRun(false);
    
    thread.start([&threadDidRun]() {
        threadDidRun = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });
    
    cr_assert(thread.isRunning(), "Le thread doit être en cours d'exécution après start");
    
    // Attendre que le thread s'exécute
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    thread.stop();
    cr_assert(!thread.isRunning(), "Le thread ne doit pas être en cours d'exécution après stop");
    cr_assert(threadDidRun, "La fonction du thread doit avoir été exécutée");
}

Test(NetworkThread, multiple_tasks) {
    NetworkThread thread;
    std::atomic<int> counter(0);
    
    thread.start([&counter]() {
        while (counter < 5) {
            counter++;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
    
    // Attendre que le compteur atteigne 5
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    thread.stop();
    cr_assert_eq(counter, 5, "Le compteur doit avoir été incrémenté à 5 par le thread");
}

Test(NetworkThread, stop_without_start) {
    NetworkThread thread;
    
    // Cela ne devrait pas causer de problèmes
    thread.stop();
    cr_assert(!thread.isRunning(), "Le thread ne doit pas être en cours d'exécution");
}
