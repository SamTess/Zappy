/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** messageQueue
*/


#include "MessageQueue.hpp"
#include <string>
#include <chrono>

MessageQueue::MessageQueue() {
}

void MessageQueue::enqueue(const std::string &message) {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(message);
    }
    _cv.notify_one();
}

std::string MessageQueue::dequeue() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_queue.empty())
        return "";
    std::string message = _queue.front();
    _queue.pop();
    return message;
}

std::string MessageQueue::dequeueWait(int timeoutMs) {
    std::unique_lock<std::mutex> lock(_mutex);
    if (timeoutMs < 0) {
        _cv.wait(lock, [this] { return !_queue.empty(); });
    } else {
        auto timeout = std::chrono::milliseconds(timeoutMs);
        bool messageAvailable = _cv.wait_for(lock, timeout, [this] { return !_queue.empty(); });
        if (!messageAvailable)
            return "";
    }
    std::string message = _queue.front();
    _queue.pop();
    return message;
}

bool MessageQueue::isEmpty() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
}

size_t MessageQueue::size() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.size();
}

void MessageQueue::clear() {
    std::lock_guard<std::mutex> lock(_mutex);
    while (!_queue.empty()) {
        _queue.pop();
    }
}
