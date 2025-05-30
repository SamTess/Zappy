/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** networkThread
*/

#ifndef NETWORKTHREAD_HPP_
#define NETWORKTHREAD_HPP_

#include <thread>
#include <functional>
#include <atomic>
#include <mutex>

class NetworkThread {
    public:
        NetworkThread();
        ~NetworkThread();

        void start(const std::function<void()>& threadFunction);
        void stop();
        bool isRunning() const;

    private:
        std::thread _thread;
        std::atomic<bool> _isRunning;
};
#endif /* !NETWORKTHREAD_HPP_ */
