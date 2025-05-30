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
        {
            std::lock_guard<std::mutex> observersLock(_observersMutex);
            for (auto* observer : _observers) {
                observer->onConnectionStatusChanged(true);
            }
        }
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
        {
            std::lock_guard<std::mutex> observersLock(_observersMutex);
            for (auto* observer : _observers) {
                observer->onConnectionStatusChanged(false);
            }
        }
    }
}

bool NetworkManager::isConnected() const {
    return _isConnected;
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
    const int maxBufferSize = 4096;
    auto buffer = std::make_unique<SystemWrapper::SafeBuffer>(maxBufferSize);
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
    notifyObservers(header, data);
}

void NetworkManager::notifyObservers(const std::string& header, const std::string& data) {
    std::lock_guard<std::mutex> lock(_observersMutex);
    for (auto* observer : _observers) {
        observer->onMessage(header, data);
    }
}

void NetworkManager::addObserver(NetworkObserver* observer) {
    if (!observer) return;
    std::lock_guard<std::mutex> lock(_observersMutex);
    if (std::find(_observers.begin(), _observers.end(), observer) == _observers.end()) {
        _observers.push_back(observer);
    }
}

void NetworkManager::removeObserver(NetworkObserver* observer) {
    if (!observer) return;
    std::lock_guard<std::mutex> lock(_observersMutex);
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}
