/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** messageQueue
*/


#include "MessageQueue.hpp"
#include <string>

MessageQueue::MessageQueue() {
}

void MessageQueue::enqueue(const std::string &message) {
    _queue.push(message);
}

std::string MessageQueue::dequeue() {
    if (_queue.empty())
        return "";
    std::string message = _queue.front();
    _queue.pop();
    return message;
}

bool MessageQueue::isEmpty() const {
    return _queue.empty();
}

size_t MessageQueue::size() const {
    return _queue.size();
}

void MessageQueue::clear() {
    while (!_queue.empty()) {
        _queue.pop();
    }
}
