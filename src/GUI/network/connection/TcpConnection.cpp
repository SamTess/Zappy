/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tcpConnection
*/

#include "TcpConnection.hpp"
#include <memory>
#include <string>

TcpConnection::TcpConnection() {
    _networkFactory = Network::createFactory();
    _socket = _networkFactory->createTcpSocket();
}

TcpConnection::~TcpConnection() {
    close();
}

void TcpConnection::connect(const std::string &host, int port) {
    createAndConfigureSocket();
    performConnect(host, port);
}

void TcpConnection::createAndConfigureSocket() {
    if (!_socket)
        _socket = _networkFactory->createTcpSocket();
    if (!_socket->setNonBlocking())
        throw TcpConnectionException("Failed to set socket non-blocking");
}

void TcpConnection::performConnect(const std::string &host, int port) {
    if (!_socket || !_socket->connect(host, port))
        throw TcpConnectionException("Connection failed to " + host + ":" + std::to_string(port));
}

void TcpConnection::send(const std::string &message) {
    std::string finalMessage;

    if (!_socket || !_socket->isConnected())
        throw TcpConnectionException("Socket is not connected");
    finalMessage = message;
    if (finalMessage.empty() || finalMessage.back() != '\n')
        finalMessage += '\n';
    if (!_socket->send(finalMessage))
        throw TcpConnectionException("Send failed");
}

std::string TcpConnection::receive() {
    if (!_socket || !_socket->isConnected())
        throw TcpConnectionException("Socket is not connected");
    try {
        if (!_socket->hasData())
            return "";
        return readDataFromSocket();
    } catch (const std::exception& e) {
        throw TcpConnectionException(std::string("Receive failed: ") + e.what());
    }
}

std::string TcpConnection::readDataFromSocket() {
    return _socket->receive();
}

void TcpConnection::close() {
    if (_socket)
        _socket->disconnect();
}

bool TcpConnection::isConnected() const {
    if (_socket)
        return _socket->isConnected();
    return false;
}

bool TcpConnection::hasData() const {
    if (_socket)
        return _socket->hasData();
    return false;
}
