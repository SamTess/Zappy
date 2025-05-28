/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** messageQueue
*/

#ifndef MESSAGEQUEUE_HPP_
#define MESSAGEQUEUE_HPP_

#include <string>
#include <queue>
#include <mutex>

class MessageQueue {
    public:
        MessageQueue();
        ~MessageQueue() = default;

        void enqueue(const std::string &message);
        std::string dequeue();
        bool isEmpty() const;
        size_t size() const;
        void clear();

    private:
        std::queue<std::string> _queue;
};

#endif /* !MESSAGEQUEUE_HPP_ */
