/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** circularBuffer
*/

#ifndef CIRCULARBUFFER_HPP_
#define CIRCULARBUFFER_HPP_

#include <memory>
#include <cstddef>
#include <string>
#include <string_view>
#include "../utils/SystemWrapper.hpp"

class CircularBuffer {
    public:
        explicit CircularBuffer(size_t capacity = 10);
        ~CircularBuffer() = default;

        void write(const std::string_view& data);
        void write(const std::string& data);
        void write(const SystemWrapper::SafeBuffer& buffer, size_t size);
        size_t read(std::string& buffer, size_t maxSize);
        std::string readString(size_t maxSize);
        SystemWrapper::SafeBuffer readSafeBuffer(size_t maxSize);
        std::string readAsString(size_t maxSize);
        size_t available() const;
        bool isEmpty() const;
        bool isFull() const;
        void clear();
        void resize(size_t newCapacity);

    private:
        std::string _buffer;
        size_t _capacity;
        size_t _head;
        size_t _tail;
        size_t _size;
};
#endif /* !CIRCULARBUFFER_HPP_ */
