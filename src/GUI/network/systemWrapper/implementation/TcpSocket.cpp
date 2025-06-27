/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** TcpSocket
*/

#include "TcpSocket.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <memory>
#include <vector>

namespace Network {
namespace Implementation {

TcpSocket::TcpSocket() : _fd(-1), _connected(false) {
    createSocket();
}

TcpSocket::~TcpSocket() {
    disconnect();
}

void TcpSocket::createSocket() {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
}

bool TcpSocket::connect(const std::string& host, int port) {
    if (_connected) {
        disconnect();
        createSocket();
    }
    try {
        _address = std::make_unique<NetworkAddress>(host, port);
        performConnect();
        _connected = true;
        return true;
    } catch (const std::exception&) {
        cleanup();
        return false;
    }
}

void TcpSocket::performConnect() {
    int result = ::connect(_fd, _address->getSockAddr(), _address->getAddrSize());
    pollfd pfd;
    int pollResult;
    int error;
    socklen_t len;

    if (result < 0 && errno != EINPROGRESS)
        throw std::runtime_error("Connection failed: " + std::string(strerror(errno)));
    if (errno == EINPROGRESS) {
        pfd = {_fd, POLLOUT, 0};
        pollResult = poll(&pfd, 1, 5000);
        if (pollResult <= 0)
            throw std::runtime_error("Connection timeout");
        error = 0;
        len = sizeof(error);
        if (getsockopt(_fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0)
            throw std::runtime_error("Connection failed: " + std::string(strerror(error)));
    }
}

bool TcpSocket::isConnected() const {
    return _connected && isValidDescriptor();
}

void TcpSocket::disconnect() {
    if (isValidDescriptor()) {
        close(_fd);
        _fd = -1;
    }
    _connected = false;
    _address.reset();
}

bool TcpSocket::send(const std::string& data) {
    std::string message;
    size_t totalSent = 0;
    pollfd pfd;
    ssize_t sent;

    if (!isConnected())
        return false;
    message = data;
    if (message.empty() || message.back() != '\n')
        message += '\n';
    const size_t size = message.size();
    while (totalSent < size) {
        pfd = {_fd, POLLOUT, 0};
        if (poll(&pfd, 1, 5000) <= 0)
            return false;
        sent = write(_fd, message.c_str() + totalSent, size - totalSent);
        if (sent < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            return false;
        }
        totalSent += sent;
    }
    return true;
}

std::string TcpSocket::receive() {
    pollfd pfd;
    ssize_t bytesRead;
    std::vector<char> buffer(1024, 0);

    if (!isConnected())
        return "";
    pfd = {_fd, POLLIN, 0};
    if (poll(&pfd, 1, 500) <= 0)
        return "";
    bytesRead = read(_fd, buffer.data(), buffer.size() - 1);
    if (bytesRead < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            return "";
        disconnect();
        return "";
    }
    if (bytesRead == 0) {
        disconnect();
        return "";
    }
    return std::string(buffer.data(), bytesRead);
}

bool TcpSocket::hasData() const {
    pollfd pfd;

    if (!isConnected())
        return false;
    pfd = {_fd, POLLIN, 0};
    int result = poll(&pfd, 1, 0);
    if (result > 0)
        return (pfd.revents & POLLIN);
    return false;
}

bool TcpSocket::setNonBlocking() {
    int flags;

    if (!isValidDescriptor())
        return false;
    flags = fcntl(_fd, F_GETFL, 0);
    if (flags < 0)
        return false;
    return fcntl(_fd, F_SETFL, flags | O_NONBLOCK) >= 0;
}

void TcpSocket::cleanup() {
    disconnect();
}

bool TcpSocket::isValidDescriptor() const {
    return _fd >= 0;
}

int TcpSocket::getFileDescriptor() const {
    return _fd;
}

} // namespace implementation
} // namespace Network
