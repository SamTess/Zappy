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
#include <memory>
#include <condition_variable>

class MessageQueue {
    public:
        MessageQueue();
        ~MessageQueue() = default;

        void enqueue(const std::string &message);
        std::string dequeue();
        std::string dequeueWait(int timeoutMs = -1);
        bool isEmpty() const;
        size_t size() const;
        void clear();

    private:
        std::queue<std::string> _queue;
        mutable std::mutex _mutex;
        std::condition_variable _cv;
};

#endif /* !MESSAGEQUEUE_HPP_ */
