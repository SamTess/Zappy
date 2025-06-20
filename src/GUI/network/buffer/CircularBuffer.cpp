/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CircularBuffer implementation
*/

#include "CircularBuffer.hpp"
#include <algorithm>
#include <cstring>
#include <cstdio>

CircularBuffer::CircularBuffer(size_t capacity)
    : _buffer(capacity, '\0'), _head(0), _tail(0), _size(0), _capacity(capacity) {
}

bool CircularBuffer::write(const std::string data, size_t size) {
    std::lock_guard<std::mutex> lock(_mutex);
    for (size_t i = 0; i < size; ++i) {
        _buffer[_head] = data[i];
        _head = nextIndex(_head);
        if (_size < _capacity) {
            ++_size;
        } else {
            _tail = nextIndex(_tail);
        }
    }
    return true;
}

bool CircularBuffer::write(const std::string& data) {
    return write(data, data.size());
}

std::string CircularBuffer::readLine() {
    std::lock_guard<std::mutex> lock(_mutex);
    std::string result;
    size_t currentTail = _tail;
    size_t elementsRead = 0;
    while (elementsRead < _size) {
        char c = _buffer[currentTail];
        currentTail = nextIndex(currentTail);
        ++elementsRead;
        if (c == '\n') {
            for (size_t i = 0; i < elementsRead; ++i) {
                result += _buffer[_tail];
                _tail = nextIndex(_tail);
                --_size;
            }
            if (!result.empty() && result.back() == '\n')
                result.pop_back();
            return result;
        }
    }
    return "";
}

size_t CircularBuffer::size() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _size;
}

size_t CircularBuffer::capacity() const {
    return _capacity;
}

bool CircularBuffer::empty() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _size == 0;
}

bool CircularBuffer::full() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _size == _capacity;
}

void CircularBuffer::clear() {
    std::lock_guard<std::mutex> lock(_mutex);
    _head = 0;
    _tail = 0;
    _size = 0;
}

bool CircularBuffer::hasLine() const {
    std::lock_guard<std::mutex> lock(_mutex);

    size_t currentTail = _tail;
    for (size_t i = 0; i < _size; ++i) {
        if (_buffer[currentTail] == '\n') {
            return true;
        }
        currentTail = nextIndex(currentTail);
    }
    return false;
}

size_t CircularBuffer::nextIndex(size_t index) const {
    return (index + 1) % _capacity;
}
