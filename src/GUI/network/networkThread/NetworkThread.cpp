/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** networkThread
*/

#include "NetworkThread.hpp"

NetworkThread::NetworkThread() : _isRunning(false) {
}

NetworkThread::~NetworkThread() {
    stop();
}

void NetworkThread::start(const std::function<void()>& threadFunction) {
    if (!_isRunning) {
        _isRunning = true;
        _thread = std::thread(threadFunction);
    }
}

void NetworkThread::stop() {
    if (_isRunning) {
        _isRunning = false;
        if (_thread.joinable())
            _thread.join();
    }
}

bool NetworkThread::isRunning() const {
    return _isRunning;
}

