/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Buffer
*/

#include "Buffer.hpp"
#include <algorithm>
#include <string>

namespace Network {
namespace Implementation {

Buffer::Buffer(size_t initialSize) : _capacity(initialSize) {
    _data.reserve(_capacity);
}

std::string& Buffer::getData() {
    return _data;
}

const std::string& Buffer::getData() const {
    return _data;
}

void Buffer::setData(const std::string& data) {
    _data = data;
    if (_data.capacity() > _capacity)
        _capacity = _data.capacity();
}

size_t Buffer::size() const {
    return _data.size();
}

void Buffer::resize(size_t newSize) {
    _data.resize(newSize);
    _capacity = std::max(_capacity, newSize);
}

void Buffer::clear() {
    _data.clear();
}

std::string Buffer::toString() const {
    return _data;
}

} // namespace implementation
} // namespace Network
