/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** circularBuffer
*/

#include "CircularBuffer.hpp"
#include <algorithm>
#include <utility>
#include <vector>

CircularBuffer::CircularBuffer(size_t capacity)
    : _capacity(capacity), _head(0), _tail(0), _size(0) {
    _buffer.resize(_capacity);
}

void CircularBuffer::write(const char* data, size_t size) {
    if (size == 0)
        return;
    if (size > _capacity - _size)
        resize(_capacity + size);
    for (size_t i = 0; i < size; ++i) {
        _buffer[_tail] = data[i];
        _tail = (_tail + 1) % _capacity;
        if (_size < _capacity)
            _size++;
        else
            _head = (_head + 1) % _capacity;
    }
}

size_t CircularBuffer::read(char* buffer, size_t maxSize) {
    if (isEmpty() || maxSize == 0)
        return 0;
    size_t bytesToRead = std::min(maxSize, _size);
    size_t bytesRead = 0;

    while (bytesRead < bytesToRead) {
        buffer[bytesRead] = _buffer[_head];
        _head = (_head + 1) % _capacity;
        bytesRead++;
    }
    _size -= bytesRead;
    return bytesRead;
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

void CircularBuffer::resize(size_t newCapacity) {
    if (newCapacity <= _capacity)
        return;
    std::vector<char> newBuffer(newCapacity);
    size_t count = 0;

    for (size_t i = 0; i < _size; ++i) {
        size_t index = (_head + i) % _capacity;
        newBuffer[i] = _buffer[index];
        count++;
    }
    _buffer = std::move(newBuffer);
    _capacity = newCapacity;
    _head = 0;
    _tail = count;
}

void CircularBuffer::clear() {
    _head = 0;
    _tail = 0;
    _size = 0;
}
