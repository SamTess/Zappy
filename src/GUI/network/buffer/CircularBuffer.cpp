/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** circularBuffer
*/

#include "CircularBuffer.hpp"

CircularBuffer::CircularBuffer(size_t capacity)
    : _capacity(capacity), _head(0), _tail(0), _size(0) {
    _buffer.resize(_capacity);
}

void CircularBuffer::write(const char* data, size_t size) {
    (void)data;
    (void)size;
    // TODO: Implement circular buffer write
}

size_t CircularBuffer::read(char* buffer, size_t maxSize) {
    (void)buffer;
    (void)maxSize;
    // TODO: Implement circular buffer read
    return 0;
}

size_t CircularBuffer::available() const {
    return _size;
}

bool CircularBuffer::isEmpty() const {
    return _size == 0;
}

bool CircularBuffer::isFull() const {
    return _size == _capacity;
}

void CircularBuffer::clear() {
    _head = 0;
    _tail = 0;
    _size = 0;
}
