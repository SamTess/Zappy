/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tcpConnection
*/

#include "TcpConnection.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <string>
#include <cstring>

TcpConnection::TcpConnection() : _socket(-1) {
    _pollfd.fd = -1;
    _pollfd.events = 0;
    _pollfd.revents = 0;
    _sockaddr = {};
}

TcpConnection::~TcpConnection() {
    close();
}

void TcpConnection::connect(const std::string &host, int port) {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
        throw TcpConnectionException("Failed to create socket: " + std::string(strerror(errno)));

    int flags = fcntl(_socket, F_GETFL, 0);
    if (flags == -1 || fcntl(_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        close();
        throw TcpConnectionException("Failed to set socket non-blocking: " + std::string(strerror(errno)));
    }

    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &_sockaddr.sin_addr) <= 0) {
        close();
        throw TcpConnectionException("Invalid IP address: " + host);
    }

    int connectResult = ::connect(_socket, reinterpret_cast<struct sockaddr*>(&_sockaddr), sizeof(_sockaddr));
    if (connectResult < 0 && errno != EINPROGRESS) {
        close();
        throw TcpConnectionException("Connection failed: " + std::string(strerror(errno)));
    }


    if (errno == EINPROGRESS) {
        struct pollfd tempPollfd;
        tempPollfd.fd = _socket;
        tempPollfd.events = POLLOUT;
        tempPollfd.revents = 0;

        int pollResult = poll(&tempPollfd, 1, 5000); // 5 seconds timeout
        if (pollResult <= 0) {
            close();
            throw TcpConnectionException("Connection timeout or poll error");
        }
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
            close();
            throw TcpConnectionException("Connection failed: " + std::string(strerror(error)));
        }
    }
    _pollfd.fd = _socket;
    _pollfd.events = POLLIN | POLLOUT;
    _pollfd.revents = 0;
}

void TcpConnection::send(const std::string &message) {
    if (_socket < 0)
        throw TcpConnectionException("Socket is not connected");

    size_t totalSent = 0;
    const char* buffer = message.c_str();
    size_t size = message.size();

    while (totalSent < size) {
        _pollfd.events = POLLOUT;
        _pollfd.revents = 0;

        int pollResult = poll(&_pollfd, 1, 5000);
        if (pollResult <= 0)
            throw TcpConnectionException("Send timeout or poll error");

        if (!(_pollfd.revents & POLLOUT))
            throw TcpConnectionException("Socket not ready for writing");

        ssize_t sent = write(_socket, buffer + totalSent, size - totalSent);
        if (sent < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            throw TcpConnectionException("Send error: " + std::string(strerror(errno)));
        }

        totalSent += sent;
    }
}

std::string TcpConnection::receive() {
    if (_socket < 0)
        throw TcpConnectionException("Socket is not connected");

    _pollfd.events = POLLIN;
    _pollfd.revents = 0;

    int pollResult = poll(&_pollfd, 1, 5000);
    if (pollResult <= 0)
        throw TcpConnectionException("Receive timeout or poll error");

    if (!(_pollfd.revents & POLLIN))
        throw TcpConnectionException("No data available");

    char buffer[1024];
    ssize_t bytesReceived = read(_socket, buffer, sizeof(buffer) - 1);
    if (bytesReceived < 0) {
        if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            return "";
        throw TcpConnectionException("Receive error: " + std::string(strerror(errno)));
    }

    if (bytesReceived == 0)
        throw TcpConnectionException("Connection closed by server");

    buffer[bytesReceived] = '\0';
    return std::string(buffer, bytesReceived);
}

void TcpConnection::close() {
    if (_socket >= 0) {
        ::close(_socket);
        _socket = -1;
        _pollfd.fd = -1;
        _pollfd.events = 0;
        _pollfd.revents = 0;
    }
}

bool TcpConnection::hasData() const {
    if (_socket < 0)
        return false;

    struct pollfd tempPollfd;
    tempPollfd.fd = _socket;
    tempPollfd.events = POLLIN;
    tempPollfd.revents = 0;

    int pollResult = poll(&tempPollfd, 1, 0);  // a changer si on veux faire un poll inteligent
    return pollResult > 0 && (tempPollfd.revents & POLLIN);
}

bool TcpConnection::isConnected() const {
    return _socket >= 0;
}
