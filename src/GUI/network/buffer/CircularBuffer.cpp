/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** circularBuffer
*/

#include "CircularBuffer.hpp"
#include <algorithm>
#include <utility>
#include <memory>
#include <string>

CircularBuffer::CircularBuffer(size_t capacity)
    : _buffer(capacity, '\0'),
      _capacity(capacity), _head(0), _tail(0), _size(0) {
}

void CircularBuffer::write(const std::string_view& data) {
    if (data.empty())
        return;
    size_t size = data.size();
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

void CircularBuffer::write(const std::string& data) {
    write(std::string_view{data});
}

void CircularBuffer::write(std::shared_ptr<SystemWrapper::SafeBuffer> buffer, size_t size) {
    if (buffer) {
        write(buffer->data().substr(0, std::min(size, buffer->size())));
    }
}

std::string CircularBuffer::readString(size_t maxSize) {
    if (isEmpty() || maxSize == 0)
        return "";
    size_t bytesToRead = std::min(maxSize, _size);
    std::string result;
    result.reserve(bytesToRead);
    for (size_t i = 0; i < bytesToRead; ++i) {
        result.push_back(_buffer[_head]);
        _head = (_head + 1) % _capacity;
    }
    _size -= bytesToRead;
    return result;
}

size_t CircularBuffer::read(std::shared_ptr<std::string> buffer, size_t maxSize) {
    if (isEmpty() || maxSize == 0)
        return 0;
    size_t bytesToRead = std::min(maxSize, _size);
    buffer->clear();
    buffer->reserve(bytesToRead);
    for (size_t i = 0; i < bytesToRead; ++i) {
        buffer->push_back(_buffer[_head]);
        _head = (_head + 1) % _capacity;
    }
    _size -= bytesToRead;
    return bytesToRead;
}

SystemWrapper::SafeBuffer CircularBuffer::readSafeBuffer(size_t maxSize) {
    std::string str = readString(maxSize);
    SystemWrapper::SafeBuffer buf(str.size());
    buf.data() = str;
    return buf;
}

std::string CircularBuffer::readAsString(size_t maxSize) {
    return readString(maxSize);
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
    std::string newBuffer(newCapacity, '\0');
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
