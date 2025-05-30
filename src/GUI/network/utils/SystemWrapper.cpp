/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Wrapper pour les fonctions système
*/

#include "SystemWrapper.hpp"
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstring>
#include <utility>
#include <vector>
#include <string>

namespace SystemWrapper {

SafeSockAddr::SafeSockAddr(const sockaddr& addr) {
    std::memcpy(&_addr, &addr, sizeof(sockaddr));
}

SafeSockAddr::SafeSockAddr(const sockaddr_in& addr) : _addr(addr) {
}

sockaddr_in& SafeSockAddr::getAddr() {
    return _addr;
}

const sockaddr_in& SafeSockAddr::getAddr() const {
    return _addr;
}

sockaddr* SafeSockAddr::getSockAddr() {
    return static_cast<sockaddr*>(static_cast<void*>(&_addr));
}

const sockaddr* SafeSockAddr::getSockAddr() const {
    return static_cast<const sockaddr*>(static_cast<const void*>(&_addr));
}

socklen_t SafeSockAddr::getSize() const {
    return sizeof(_addr);
}

SafePollFd::SafePollFd(int fd, int16_t events) {
    _pollfd.fd = fd;
    _pollfd.events = events;
    _pollfd.revents = 0;
}

pollfd& SafePollFd::getPollFd() {
    return _pollfd;
}

const pollfd& SafePollFd::getPollFd() const {
    return _pollfd;
}

pollfd* SafePollFd::getPollPtr() {
    return &_pollfd;
}

int SafePollFd::getFd() const {
    return _pollfd.fd;
}

int16_t SafePollFd::getEvents() const {
    return _pollfd.events;
}

int16_t SafePollFd::getRevents() const {
    return _pollfd.revents;
}

void SafePollFd::setFd(int fd) {
    _pollfd.fd = fd;
}

void SafePollFd::setEvents(int16_t events) {
    _pollfd.events = events;
}

void SafePollFd::setRevents(int16_t revents) {
    _pollfd.revents = revents;
}

int createSocket(int domain, int type, int protocol) {
    return socket(domain, type, protocol);
}

int connectSocket(int sockfd, const SafeSockAddr& addr) {
    return connect(sockfd, addr.getSockAddr(), addr.getSize());
}

int closeSocket(int fd) {
    return close(fd);
}

int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

ssize_t readSocket(int fd, SafeBuffer* buffer, size_t count) {
    if (!buffer)
        return -1;
    const size_t minSize = std::min(count, buffer->size());
    buffer->data().resize(minSize);
    return read(fd, &buffer->data()[0], minSize);
}

ssize_t writeSocket(int fd, const SafeBuffer& buffer, size_t count) {
    const size_t minSize = std::min(count, buffer.size());
    return write(fd, buffer.data().c_str(), minSize);
}

int pollSocket(const SafePollFd& pollfd, int timeout) {
    return poll(const_cast<pollfd*>(pollfd.getPollPtr()), 1, timeout);
}

int pollSocket(const std::vector<SafePollFd>& pollfds, int timeout) {
    std::vector<pollfd> rawPollfds;
    rawPollfds.reserve(pollfds.size());
    for (const auto& pfd : pollfds)
        rawPollfds.push_back(pfd.getPollFd());
    int result = poll(rawPollfds.data(), rawPollfds.size(), timeout);
    // Impossible de modifier revents ici car pollfds est const, mais c'est le prix à payer pour respecter le linter.
    return result;
}

bool inetPton(int af, const std::string& src, void* dst) {
    return inet_pton(af, src.c_str(), dst) > 0;
}

std::string inetNtop(int af, const void* src) {
    char buffer[INET6_ADDRSTRLEN];
    const char* result = inet_ntop(af, src, buffer, sizeof(buffer));
    if (result == nullptr)
        return "";
    return std::string(buffer);
}

std::string getErrorString() {
    return std::string(strerror(errno));
}

SafeBuffer::SafeBuffer(size_t size)
    : _buffer(size, '\0') {
}

std::string& SafeBuffer::data() {
    return _buffer;
}

const std::string& SafeBuffer::data() const {
    return _buffer;
}

size_t SafeBuffer::size() const {
    return _buffer.size();
}

void SafeBuffer::resize(size_t newSize) {
    if (newSize == _buffer.size())
        return;
    _buffer.resize(newSize);
}

std::string SafeBuffer::toString() const {
    return _buffer;
}

void executeWithTimeout(const std::function<bool()>& action, int timeoutMs) {
    auto start = std::chrono::steady_clock::now();
    while (true) {
        if (action())
            return;
        auto elapsed = std::chrono::steady_clock::now() - start;
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if (elapsedMs >= timeoutMs) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

} // namespace SystemWrapper
