/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tcpConnection
*/

#include "TcpConnection.hpp"
#include <memory>
#include <functional>
#include <string>

TcpConnection::TcpConnection() : _socket(-1) {
    _pollfd = std::make_unique<SystemWrapper::SafePollFd>(-1);
    _sockaddr = std::make_unique<SystemWrapper::SafeSockAddr>();
    _recvBuffer = std::make_unique<SystemWrapper::SafeBuffer>(1024);
}

TcpConnection::~TcpConnection() {
    close();
}

void TcpConnection::connect(const std::string &host, int port) {
    _socket = SystemWrapper::createSocket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
        throw TcpConnectionException("Failed to create socket: " + SystemWrapper::getErrorString());

    if (SystemWrapper::setNonBlocking(_socket) < 0) {
        close();
        throw TcpConnectionException("Failed to set socket non-blocking: " + SystemWrapper::getErrorString());
    }

    auto& addr = _sockaddr->getAddr();
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (!SystemWrapper::inetPton(AF_INET, host, &addr.sin_addr)) {
        close();
        throw TcpConnectionException("Invalid IP address: " + host);
    }

    int connectResult = SystemWrapper::connectSocket(_socket, *_sockaddr);
    if (connectResult < 0 && errno != EINPROGRESS) {
        close();
        throw TcpConnectionException("Connection failed: " + SystemWrapper::getErrorString());
    }

    if (errno == EINPROGRESS) {
        auto tempPollfd = SystemWrapper::SafePollFd(_socket, POLLOUT);

        int pollResult = SystemWrapper::pollSocket(tempPollfd, 5000); // 5 seconds timeout
        if (pollResult <= 0) {
            close();
            throw TcpConnectionException("Connection timeout or poll error");
        }
        int error = 0;
        if (SystemWrapper::getSocketOption(_socket, SOL_SOCKET, SO_ERROR, error) < 0 || error != 0) {
            close();
            throw TcpConnectionException("Connection failed: " + std::string(strerror(error)));
        }
    }
    _pollfd = std::make_unique<SystemWrapper::SafePollFd>(_socket, POLLIN | POLLOUT);
}

void TcpConnection::send(const std::string &message) {
    if (_socket < 0)
        throw TcpConnectionException("Socket is not connected");

    size_t totalSent = 0;
    const size_t size = message.size();
    auto sendBuffer = SystemWrapper::SafeBuffer(size);
    sendBuffer.data() = message;

    while (totalSent < size) {
        _pollfd->setEvents(POLLOUT);
        int pollResult = SystemWrapper::pollSocket(*_pollfd, 5000);
        if (pollResult <= 0)
            throw TcpConnectionException("Send timeout or poll error");

        if (!(_pollfd->getRevents() & POLLOUT))
            throw TcpConnectionException("Socket not ready for writing");

        ssize_t sent = SystemWrapper::writeSocket(_socket, sendBuffer, size - totalSent);
        if (sent < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            throw TcpConnectionException("Send error: " + SystemWrapper::getErrorString());
        }

        totalSent += sent;
    }
}

std::string TcpConnection::receive() {
    if (_socket < 0)
        throw TcpConnectionException("Socket is not connected");

    _pollfd->setEvents(POLLIN);
    int pollResult = SystemWrapper::pollSocket(*_pollfd, 5000);
    if (pollResult <= 0)
        throw TcpConnectionException("Receive timeout or poll error");

    if (!(_pollfd->getRevents() & POLLIN))
        throw TcpConnectionException("No data available");

    ssize_t bytesReceived = SystemWrapper::readSocket(_socket, *_recvBuffer, _recvBuffer->size() - 1);
    if (bytesReceived < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            return "";
        throw TcpConnectionException("Receive error: " + SystemWrapper::getErrorString());
    }

    if (bytesReceived == 0)
        throw TcpConnectionException("Connection closed by server");

    return _recvBuffer->data().substr(0, bytesReceived);
}

void TcpConnection::close() {
    if (_socket >= 0) {
        SystemWrapper::closeSocket(_socket);
        _socket = -1;
        _pollfd = std::make_unique<SystemWrapper::SafePollFd>(-1);
    }
}

bool TcpConnection::isConnected() const {
    return _socket >= 0;
}

bool TcpConnection::hasData() const {
    if (_socket < 0)
        return false;

    auto tempPollfd = SystemWrapper::SafePollFd(_socket, POLLIN);
    int pollResult = SystemWrapper::pollSocket(tempPollfd, 0);
    return pollResult > 0 && (tempPollfd.getRevents() & POLLIN);
}
