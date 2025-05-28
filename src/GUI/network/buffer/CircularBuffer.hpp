/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** circularBuffer
*/

#ifndef CIRCULARBUFFER_HPP_
#define CIRCULARBUFFER_HPP_

#include <vector>
#include <cstddef>

class CircularBuffer {
    public:
        CircularBuffer(size_t capacity = 10);
        ~CircularBuffer() = default;

        void write(const char* data, size_t size);
        size_t read(char* buffer, size_t maxSize);
        size_t available() const;
        bool isEmpty() const;
        bool isFull() const;
        void clear();

    private:
        std::vector<char> _buffer;
        size_t _capacity;
        size_t _head;
        size_t _tail;
        size_t _size;
};
#endif /* !CIRCULARBUFFER_HPP_ */
