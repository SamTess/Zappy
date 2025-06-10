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
    : _connection(std::make_unique<TcpConnection>()),
      _protocolParser(std::make_unique<ProtocolParser>()),
      _networkThread(std::make_unique<NetworkThread>()),
      _incomingQueue(std::make_unique<MessageQueue>()),
      _outgoingQueue(std::make_unique<MessageQueue>()),
      _receiveBuffer(""),
      _graphicalContext(std::make_unique<GraphicalContext>()),
      _isConnected(false) {
}

NetworkManager::~NetworkManager() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _isConnected = false;
    }
    if (_networkThread && _networkThread->isRunning()) {
        _networkThread->stop();
    }
    if (_connection) {
        _connection->close();
    }
}

bool NetworkManager::connect(const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (_isConnected) {
        NetworkLogger::get().log(std::string("[INFO] Already connected to ") + host + ":" + std::to_string(port));
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
        NetworkLogger::get().log(std::string("[ERROR] Connection error: ") + e.what());
        _isConnected = false;
        return false;
    }
}

void NetworkManager::disconnect() {
    bool wasConnected = false;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        wasConnected = _isConnected;
        _isConnected = false;
    }
    if (wasConnected) {
        _networkThread->stop();
        _connection->close();
        if (_connectionCallback)
            _connectionCallback(false);
    }
}

bool NetworkManager::isConnected() const {
    return _isConnected && _connection->isConnected();
}

void NetworkManager::sendCommand(const std::string& command) {
    if (!validateConnectionForSending()) {
        return;
    }
    std::string finalCommand = formatCommand(command);
    logOutgoingCommand(finalCommand);
    queueCommandForSending(finalCommand);
}

bool NetworkManager::validateConnectionForSending() {
    bool connected;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        connected = _isConnected;
    }
    if (!connected) {
        NetworkLogger::get().log("[ERROR] Cannot send command: not connected");
        return false;
    }
    return true;
}

std::string NetworkManager::formatCommand(const std::string& command) {
    std::string finalCommand = command;
    if (finalCommand.empty() || finalCommand.back() != '\n') {
        finalCommand += '\n';
    }
    return finalCommand;
}

void NetworkManager::logOutgoingCommand(const std::string& formattedCommand) {
    NetworkLogger::get().log(std::string("[SEND] ") + formattedCommand);
}

void NetworkManager::queueCommandForSending(const std::string& formattedCommand) {
    _outgoingQueue->enqueue(formattedCommand);
}

void NetworkManager::networkThreadLoop() {
    int errorCount = 0;
    const int maxErrors = 3;
    NetworkLogger::get().log("Network thread started");
    if (!tryReceiveInitialWelcome()) {
        NetworkLogger::get().log("Failed to receive WELCOME message, exiting network thread");
        return;
    }
    while (_networkThread->isRunning()) {
        if (!_isConnected) {
            NetworkLogger::get().log("Network thread: connection lost, exiting...");
            break;
        }
        try {
            errorCount = receiveAndProcessData(errorCount, maxErrors);
            processIncomingMessages();
            errorCount = processPendingOutgoingMessages(errorCount, maxErrors);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } catch (const std::exception& e) {
            errorCount = handleNetworkThreadError(errorCount, maxErrors, e);
            if (errorCount >= maxErrors) {
                break;
            }
        }
    }
    NetworkLogger::get().log("Network thread exited");
}

bool NetworkManager::processInitialWelcomeData() {
    size_t pos = 0;
    while ((pos = _receiveBuffer.find('\n')) != std::string::npos) {
        std::string message = _receiveBuffer.substr(0, pos + 1);
        _receiveBuffer.erase(0, pos + 1);
        NetworkLogger::get().log(std::string("Initial message extracted: ") + message);
        if (message.find("WELCOME") != std::string::npos) {
            NetworkLogger::get().log("WELCOME message received during initialization");
            handleWelcomeMessage(message);
            return true;
        } else {
            _incomingQueue->enqueue(message);
        }
    }
    return false;
}

bool NetworkManager::tryReceiveInitialWelcome() {
    try {
        for (int i = 0; i < 30 && _networkThread->isRunning(); ++i) {
            std::string data = _connection->receive();
            if (!data.empty()) {
                _receiveBuffer += data;
                if (processInitialWelcomeData()) {
                    processPendingOutgoingMessages(0, 3);
                    return true;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        NetworkLogger::get().log("[ERROR] Timeout waiting for WELCOME message");
        return false;
    } catch (const std::exception& e) {
        NetworkLogger::get().log(std::string("[ERROR] Error receiving initial welcome: ") + e.what());
        return false;
    }
}

int NetworkManager::receiveAndProcessData(int errorCount, int maxErrors) {
    try {
        std::string data = _connection->receive();
        if (!data.empty()) {
            NetworkLogger::get().log(std::string("Data received (") +
            std::to_string(data.size()) + " bytes): " + (data.size() > 20 ? data.substr(0, 20) + "..." : data));
            _receiveBuffer += data;
            extractCompleteMessages();
            return 0;
        }
        return errorCount;
    } catch (const std::exception& e) {
        return handleReceiveError(errorCount, maxErrors, e);
    }
}

void NetworkManager::extractCompleteMessages() {
    size_t pos = 0;
    while ((pos = _receiveBuffer.find('\n')) != std::string::npos) {
        std::string message = _receiveBuffer.substr(0, pos + 1);
        _receiveBuffer.erase(0, pos + 1);
        if (!message.empty()) {
            NetworkLogger::get().log(std::string("Message extracted: ") + message);
            _incomingQueue->enqueue(message);
        }
    }
}

int NetworkManager::handleReceiveError(int errorCount, int maxErrors, const std::exception& e) {
    const std::string errorMsg = e.what();
    if (errorMsg.find("Connection closed") != std::string::npos) {
        NetworkLogger::get().log(std::string("[ERROR] Server closed connection: ") + errorMsg);
        std::lock_guard<std::mutex> lock(_mutex);
        _isConnected = false;
        return maxErrors;
    } else if (errorMsg.find("Timeout") == std::string::npos) {
        errorCount++;
        NetworkLogger::get().log(std::string("[ERROR] Error receiving data: ") + errorMsg + " (error " + std::to_string(errorCount) + "/" + std::to_string(maxErrors) + ")");
        if (errorCount >= maxErrors) {
            NetworkLogger::get().log("[ERROR] Too many receive errors, disconnecting");
            std::lock_guard<std::mutex> lock(_mutex);
            _isConnected = false;
        }
    }
    return errorCount;
}

int NetworkManager::processPendingOutgoingMessages(int errorCount, int maxErrors) {
    if (_outgoingQueue->isEmpty())
        return errorCount;
    std::string message = _outgoingQueue->dequeue();
    if (message.empty())
        return errorCount;
    try {
        _connection->send(message);
        NetworkLogger::get().log(std::string("Message sent: ") + message);
        return errorCount;
    } catch (const std::exception& e) {
        NetworkLogger::get().log(std::string("[ERROR] Failed to send command: ") + e.what());
        errorCount++;
        if (errorCount >= maxErrors) {
            NetworkLogger::get().log("[ERROR] Too many send errors, disconnecting");
            std::lock_guard<std::mutex> lock(_mutex);
            _isConnected = false;
        }
        return errorCount;
    }
}

int NetworkManager::handleNetworkThreadError(int errorCount, int maxErrors, const std::exception& e) {
    NetworkLogger::get().log(std::string("[ERROR] Network thread error: ") + e.what());
    errorCount++;
    if (errorCount >= maxErrors) {
        NetworkLogger::get().log("[ERROR] Too many network thread errors, disconnecting");
        std::lock_guard<std::mutex> lock(_mutex);
        _isConnected = false;
    }
    return errorCount;
}

void NetworkManager::processIncomingMessages() {
    std::string message;
    while (!(message = _incomingQueue->dequeue()).empty()) {
        NetworkLogger::get().log(std::string("Processing incoming message: ") + message);
        processIncomingMessage(message);
    }
}

void NetworkManager::processIncomingMessage(const std::string& message) {
    NetworkLogger::get().log(std::string("[RECV] ") + message);
    try {
        handleRegularMessage(message);
    } catch (const std::exception& e) {
        handleInvalidMessage(message, e);
    }
}

void NetworkManager::handleWelcomeMessage(const std::string& message) {
    NetworkLogger::get().log(std::string("Message de bienvenue reçu: ") + message);
    NetworkLogger::get().log("Envoi automatique de la commande GRAPHIC suite au WELCOME");
    sendCommand("GRAPHIC");
    MessageCallback localCallback;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        localCallback = _messageCallback;
    }
    if (localCallback) {
        try {
            localCallback("WELCOME", "");
        } catch (const std::exception& e) {
            NetworkLogger::get().log(std::string("[ERROR] Error in WELCOME callback: ") + e.what());
        }
    }
}

void NetworkManager::handleRegularMessage(const std::string& message) {
    Message parsedMessage = _protocolParser->parseMessage(message);

    if (_graphicalContext)
        _graphicalContext->updateContext(parsedMessage);
    MessageCallback localCallback;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        localCallback = _messageCallback;
    }
    if (localCallback) {
        try {
            const ProtocolParser* constParser = _protocolParser.get();
            std::pair<std::string, std::string> cmdParams = constParser->parseMessage(message);
            localCallback(cmdParams.first, cmdParams.second);
        } catch (const std::exception& e) {
            NetworkLogger::get().log(std::string("[ERROR] Error in message callback: ") + e.what());
            try {
                localCallback("RAW", message);
            } catch (const std::exception& e2) {
                NetworkLogger::get().log(std::string("[ERROR] Error in raw message callback: ") + e2.what());
            }
        }
    }
}

void NetworkManager::handleInvalidMessage(const std::string& message, const std::exception& e) {
    NetworkLogger::get().log(std::string("[ERROR] Error processing message: ") + e.what());
    MessageCallback localCallback;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        localCallback = _messageCallback;
    }
    if (localCallback) {
        try {
            localCallback("RAW", message);
        } catch (const std::exception& e) {
            NetworkLogger::get().log(std::string("[ERROR] Error in raw message callback: ") + e.what());
        }
    }
}

void NetworkManager::setMessageCallback(MessageCallback callback) {
    _messageCallback = callback;
}

void NetworkManager::setConnectionCallback(ConnectionCallback callback) {
    _connectionCallback = callback;
}
