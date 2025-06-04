/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tcpConnection
*/

#include "TcpConnection.hpp"
#include <netdb.h>
#include <memory>
#include <string>
#include <chrono>
#include <thread>

TcpConnection::TcpConnection(size_t initialBufferSize) : _socket(-1), _initialBufferSize(initialBufferSize) {
    _pollfd = std::make_unique<SystemWrapper::SafePollFd>(-1);
    _sockaddr = std::make_unique<SystemWrapper::SafeSockAddr>();
    _recvBuffer = std::make_unique<SystemWrapper::SafeBuffer>(_initialBufferSize);
}

TcpConnection::~TcpConnection() {
    close();
}

void TcpConnection::connect(const std::string &host, int port) {
    if (_socket >= 0)
        close();
    createAndConfigureSocket();
    resolveAddress(host, port);
    performConnect();
    waitForConnection(5000);
    _pollfd = std::make_unique<SystemWrapper::SafePollFd>(_socket, POLLIN | POLLOUT);
}

void TcpConnection::createAndConfigureSocket() {
    _socket = SystemWrapper::createSocket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
        throw TcpConnectionException("Failed to create socket: " + SystemWrapper::getErrorString());
    if (SystemWrapper::setNonBlocking(_socket) < 0) {
        close();
        throw TcpConnectionException("Failed to set socket non-blocking: " + SystemWrapper::getErrorString());
    }
}

void TcpConnection::resolveAddress(const std::string &host, int port) {
    auto& addr = _sockaddr->getAddr();
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (isalpha(host[0])) {
        struct hostent *server = gethostbyname(host.c_str());
        if (server == nullptr) {
            close();
            throw TcpConnectionException("Unknown host: " + host);
        }
        std::memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
    } else {
        if (!SystemWrapper::inetPton(AF_INET, host, &addr.sin_addr)) {
            close();
            throw TcpConnectionException("Invalid IP address: " + host);
        }
    }
}

void TcpConnection::performConnect() {
    int connectResult = SystemWrapper::connectSocket(_socket, *_sockaddr);
    if (connectResult < 0 && errno != EINPROGRESS) {
        close();
        throw TcpConnectionException("Connection failed: " + SystemWrapper::getErrorString());
    }
}

void TcpConnection::waitForConnection(int timeoutMs) {
    if (errno == EINPROGRESS) {
        auto tempPollfd = SystemWrapper::SafePollFd(_socket, POLLOUT);
        int pollResult = SystemWrapper::pollSocket(tempPollfd, timeoutMs);
        if (pollResult <= 0) {
            close();
            throw TcpConnectionException("Connection timeout or poll error");
        }
        int error = 0;
        if (SystemWrapper::getSocketOption(_socket, SOL_SOCKET, SO_ERROR, &error) < 0 || error != 0) {
            close();
            throw TcpConnectionException("Connection failed: " + std::string(strerror(error)));
        }
    }
}

void TcpConnection::send(const std::string &message) {
    if (_socket < 0)
        throw TcpConnectionException("Socket is not connected");
    std::string finalMessage = message;
    if (finalMessage.empty() || finalMessage.back() != '\n') {
        finalMessage += '\n';
    }
    size_t totalSent = 0;
    const size_t size = finalMessage.size();
    auto sendBuffer = SystemWrapper::SafeBuffer(size);
    std::shared_ptr<std::string> bufferData = std::make_shared<std::string>(sendBuffer.data());
    bufferData = std::make_shared<std::string>(finalMessage);

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
    try {
        if (!waitForReadData(500))
            return "";
        return readDataFromSocket();
    } catch (const std::exception& e) {
        throw TcpConnectionException(std::string("Receive failed: ") + e.what());
    }
}

bool TcpConnection::waitForReadData(int timeoutMs) {
    _pollfd->setEvents(POLLIN);
    int pollResult = SystemWrapper::pollSocket(*_pollfd, timeoutMs);
    if (pollResult == 0)
        return false;
    if (pollResult < 0) {
        if (errno == EINTR)
            return false;
        throw TcpConnectionException("Poll error: " + SystemWrapper::getErrorString());
    }
    return (_pollfd->getRevents() & POLLIN);
}

std::string TcpConnection::readDataFromSocket() {
    ensureBufferSize();
    ssize_t bytesReceived = SystemWrapper::readSocket(_socket, _recvBuffer, _recvBuffer->size());
    if (bytesReceived < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            return "";
        throw TcpConnectionException("Receive error: " + SystemWrapper::getErrorString());
    }
    if (bytesReceived == 0)
        throw TcpConnectionException("Connection closed by server");
    adjustBufferSize(bytesReceived);
    std::shared_ptr<std::string> dataPtr = std::make_shared<std::string>(_recvBuffer->data());
    return dataPtr->substr(0, bytesReceived);
}

void TcpConnection::ensureBufferSize() {
    size_t currentSize = _recvBuffer->size();
    if (currentSize < _initialBufferSize)
        _recvBuffer->resize(_initialBufferSize);
}

void TcpConnection::adjustBufferSize(ssize_t bytesReceived) {
    if (bytesReceived > static_cast<ssize_t>(_recvBuffer->size() * 0.8)) {
        _recvBuffer->resize(_recvBuffer->size() * 2);
    } else if (bytesReceived < static_cast<ssize_t>(_recvBuffer->size() * 0.3) &&
               _recvBuffer->size() > _initialBufferSize * 2) {
        _recvBuffer->resize(_recvBuffer->size() / 2);
    }
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
    return true;
}
