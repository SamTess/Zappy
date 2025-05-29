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
#include <string>
#include "NetworkManager.hpp"
#include "NetworkLogger.hpp"

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
        _networkThreadObj = std::thread([this]() { this->networkThreadLoop(); });
    }
}

void NetworkManager::stopNetworkThread() {
    if (_isRunning) {
        _isRunning = false;
        if (_networkThreadObj.joinable())
            _networkThreadObj.join();
    }
}

void NetworkManager::networkThreadLoop() {
    while (_isRunning && _isConnected) {
        try {
            if (_connection->hasData()) {
                std::string data = _connection->receive();
                if (!data.empty()) {
                    NetworkLogger::get().log(std::string("Réception: ") + data);
                    _incomingQueue->enqueue(data);
                }
            }
            processIncomingMessages();
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Network thread: " << e.what() << std::endl;
            _isConnected = false;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void NetworkManager::processIncomingMessages() {
    while (!_incomingQueue->isEmpty()) {
        std::string message = _incomingQueue->dequeue();
        if (!message.empty()) {
            NetworkLogger::get().log(std::string("Dispatch: ") + message);
            processIncomingMessage(message);
        }
    }
}

void NetworkManager::processIncomingMessage(const std::string& message) {
    NetworkLogger::get().log(std::string("[RECV] ") + message);
}

void NetworkManager::sendCommand(const std::string& command) {
    if (_isConnected) {
        try {
            NetworkLogger::get().log(std::string("Envoi: ") + command);
            _connection->send(command);
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Send: " << e.what() << std::endl;
            _isConnected = false;
        }
    }
}
