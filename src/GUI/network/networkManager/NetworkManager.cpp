/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** networkManager
*/

#include "NetworkManager.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

NetworkManager::NetworkManager()
    : _connection(std::make_unique<TcpConnection>()),
      _protocolParser(std::make_unique<ProtocolParser>()),
      _networkThread(std::make_unique<NetworkThread>()),
      _incomingQueue(std::make_unique<MessageQueue>()),
      _receiveBuffer(std::make_unique<CircularBuffer>()),
      _isConnected(false),
      _isRunning(false) {
}

NetworkManager::~NetworkManager() {
    disconnect();
}

bool NetworkManager::connect(const std::string& host, int port) {
    if (_isConnected) {
        std::cout << "Already connected to " << host << ":" << port << std::endl;
        return true;
    }

    try {
        _connection->connect(host, port);
        _isConnected = true;
        startNetworkThread();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        _isConnected = false;
        return false;
    }
}

void NetworkManager::disconnect() {
    if (_isConnected) {
        stopNetworkThread();
        _connection->close();
        _isConnected = false;
    }
}

bool NetworkManager::isConnected() const {
    return _isConnected && _connection->isConnected();
}

void NetworkManager::startNetworkThread() {
    if (!_isRunning) {
        _isRunning = true;
        // Start the network thread
    }
}

void NetworkManager::stopNetworkThread() {
    if (_isRunning) {
        _isRunning = false;
        // Stop the network thread
    }
}

void NetworkManager::networkThreadLoop() {
    while (_isRunning && _isConnected) {
        try {
            if (_connection->hasData()) {
                std::string data = _connection->receive();
                if (!data.empty()) {
                    _incomingQueue->enqueue(data);
                }
            }
            processIncomingMessages();
        } catch (const std::exception& e) {
            std::cerr << "Network thread error: " << e.what() << std::endl;
            _isConnected = false;
            break;
        }
    }
}

void NetworkManager::processIncomingMessages() {
    while (!_incomingQueue->isEmpty()) {
        std::string message = _incomingQueue->dequeue();
        if (!message.empty()) {
            processIncomingMessage(message);
        }
    }
}

void NetworkManager::processIncomingMessage(const std::string& message) {
    (void)message;
}

void NetworkManager::sendCommand(const std::string& command) {
    if (_isConnected) {
        _incomingQueue->enqueue(command);
    }
}
