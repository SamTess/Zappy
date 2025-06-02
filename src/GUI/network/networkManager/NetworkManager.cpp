/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** networkManager
*/

#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <utility>
#include <string>
#include <memory>
#include "NetworkManager.hpp"
#include "NetworkLogger.hpp"

NetworkManager::NetworkManager()
    : _connection(std::make_shared<TcpConnection>()),
      _protocolParser(std::make_shared<ProtocolParser>()),
      _networkThread(std::make_shared<NetworkThread>()),
      _incomingQueue(std::make_shared<MessageQueue>()),
      _receiveBuffer(std::make_shared<CircularBuffer>()),
      _isConnected(false) {
}

NetworkManager::~NetworkManager() {
    disconnect();
}

bool NetworkManager::connect(const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (_isConnected) {
        std::cout << "Already connected to " << host << ":" << port << std::endl;
        return true;
    }

    try {
        _connection->connect(host, port);
        _isConnected = true;
        _networkThread->start([this]() { this->networkThreadLoop(); });
        if (_connectionCallback)
            _connectionCallback(true);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        _isConnected = false;
        return false;
    }
}

void NetworkManager::disconnect() {
    std::lock_guard<std::mutex> lock(_mutex);

    if (_isConnected) {
        _networkThread->stop();
        _connection->close();
        _isConnected = false;
        if (_connectionCallback)
            _connectionCallback(false);
    }
}

bool NetworkManager::isConnected() const {
    return _isConnected && _connection->isConnected();
}

void NetworkManager::sendCommand(const std::string& command) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_isConnected) {
        std::cerr << "Cannot send command: not connected" << std::endl;
        return;
    }

    try {
        _connection->send(command + "\n");
    } catch (const std::exception& e) {
        std::cerr << "Failed to send command: " << e.what() << std::endl;
        disconnect();
    }
}

void NetworkManager::networkThreadLoop() {
    while (_networkThread->isRunning() && _isConnected) {
        try {
            if (_connection->hasData()) {
                std::string data = _connection->receive();
                if (!data.empty()) {
                    _receiveBuffer->write(data);
                    while (true) {
                        std::string message = _protocolParser->extractMessage(_receiveBuffer);
                        if (message.empty())
                            break;
                        _incomingQueue->enqueue(message);
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } catch (const std::exception& e) {
            std::cerr << "Network thread error: " << e.what() << std::endl;
            disconnect();
            break;
        }
    }
}

void NetworkManager::processIncomingMessages() {
    std::string message;
    while (!(message = _incomingQueue->dequeue()).empty())
        processIncomingMessage(message);
}

void NetworkManager::processIncomingMessage(const std::string& message) {
    std::pair<std::string, std::string> parsedMessage =
        static_cast<const ProtocolParser*>(_protocolParser.get())->parseMessage(message);
    std::string header = parsedMessage.first;
    std::string data = parsedMessage.second;
    if (_messageCallback)
        _messageCallback(header, data);
}

void NetworkManager::setMessageCallback(MessageCallback callback) {
    _messageCallback = callback;
}

void NetworkManager::setConnectionCallback(ConnectionCallback callback) {
    _connectionCallback = callback;
}
