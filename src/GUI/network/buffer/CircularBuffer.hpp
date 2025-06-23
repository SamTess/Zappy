/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CircularBuffer for network data
*/

#ifndef CIRCULARBUFFER_HPP_
    #define CIRCULARBUFFER_HPP_

#include <string>
#include <mutex>
#include <memory>
class CircularBuffer {
    public:
        explicit CircularBuffer(size_t capacity = 4096);
        ~CircularBuffer() = default;

        bool write(const std::string& data, size_t size);
        bool write(const std::string& data);

        std::string readLine();

        size_t size() const;
        size_t capacity() const;
        bool empty() const;
        bool full() const;

        void clear();

        bool hasLine() const;

    private:
        std::string _buffer;
        size_t _head;
        size_t _tail;
        size_t _size;
        size_t _capacity;
        mutable std::mutex _mutex;

        size_t nextIndex(size_t index) const;
};

#endif /* !CIRCULARBUFFER_HPP_ */
