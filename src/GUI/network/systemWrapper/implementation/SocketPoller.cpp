/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** SocketPoller
*/

#include "SocketPoller.hpp"
#include <algorithm>
#include <utility>
#include <cstdint>

namespace Network {
namespace Implementation {

SocketPoller::SocketPoller() {
    _pollfds.reserve(1);
}

void SocketPoller::addSocket(int fd, int events) {
    size_t index;

    if (_fdToIndex.find(fd) != _fdToIndex.end()) {
        index = _fdToIndex[fd];
        _pollfds[index].events = events;
        return;
    }
    index = _pollfds.size();
    _pollfds.push_back({fd, static_cast<int16_t>(events), 0});
    _fdToIndex[fd] = index;
}

void SocketPoller::removeSocket(int fd) {
    auto it = _fdToIndex.find(fd);
    size_t index;

    if (it == _fdToIndex.end())
        return;
    index = it->second;
    if (index < _pollfds.size() - 1) {
        std::swap(_pollfds[index], _pollfds.back());
        _fdToIndex[_pollfds[index].fd] = index;
    }
    _pollfds.pop_back();
    _fdToIndex.erase(it);
}

int SocketPoller::poll(int timeoutMs) {
    if (_pollfds.empty())
        return 0;
    for (auto& pfd : _pollfds)
        pfd.revents = 0;
    return ::poll(_pollfds.data(), _pollfds.size(), timeoutMs);
}

bool SocketPoller::hasEvent(int fd, int event) const {
    auto it = _fdToIndex.find(fd);
    size_t index;

    if (it == _fdToIndex.end())
        return false;
    index = it->second;
    return (_pollfds[index].revents & event) != 0;
}

void SocketPoller::clear() {
    _pollfds.clear();
    _fdToIndex.clear();
}

void SocketPoller::addSocket(const TcpSocket& socket, int events) {
    addSocket(socket.getFileDescriptor(), events);
}

void SocketPoller::removeSocket(const TcpSocket& socket) {
    removeSocket(socket.getFileDescriptor());
}

bool SocketPoller::hasEvent(const TcpSocket& socket, int event) const {
    return hasEvent(socket.getFileDescriptor(), event);
}

void SocketPoller::updateIndices() {
    _fdToIndex.clear();
    for (size_t i = 0; i < _pollfds.size(); ++i)
        _fdToIndex[_pollfds[i].fd] = i;
}

} // namespace implementation
} // namespace Network
