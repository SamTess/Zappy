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


NetworkManager::NetworkManager(std::shared_ptr<Logger> logger)
    : _connection(std::make_unique<TcpConnection>()),
      _protocolParser(std::make_unique<ProtocolParser>(logger)),
      _networkThread(std::make_unique<NetworkThread>()),
      _incomingQueue(std::make_unique<MessageQueue>()),
      _outgoingQueue(std::make_unique<MessageQueue>()),
      _receiveBuffer(""),
      _gameController(std::make_shared<GameController>(logger)),
      _isConnected(false),
      _logger(logger) {
    _logger->logInfo("Network Manager initialized with logger");
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
        _logger->logInfo("Already connected to " + host + ":" + std::to_string(port));
        return true;
    }

    try {
        _connection->connect(host, port);
        _isConnected = true;
        _networkThread->start([this]() { this->networkThreadLoop(); });
        if (_connectionCallback)
            _connectionCallback(true);
        _logger->logInfo("Successfully connected to " + host + ":" + std::to_string(port));
        return true;
    } catch (const std::exception& e) {
        _logger->logError("Connection error: " + std::string(e.what()));
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
        _logger->logError("Cannot send command: not connected");
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
    _logger->logDebug("SEND: " + formattedCommand.substr(0, formattedCommand.length()-1));
}

void NetworkManager::queueCommandForSending(const std::string& formattedCommand) {
    _outgoingQueue->enqueue(formattedCommand);
}

void NetworkManager::networkThreadLoop() {
    int errorCount = 0;
    const int maxErrors = 3;
    _logger->logInfo("Network thread started");
    if (!tryReceiveInitialWelcome()) {
        _logger->logError("Failed to receive WELCOME message, exiting network thread");
        return;
    }
    while (_networkThread->isRunning()) {
        if (!_isConnected) {
            _logger->logInfo("Network thread: connection lost, exiting...");
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
    _logger->logInfo("Network thread exited");
}

bool NetworkManager::processInitialWelcomeData() {
    size_t pos = 0;
    while ((pos = _receiveBuffer.find('\n')) != std::string::npos) {
        std::string message = _receiveBuffer.substr(0, pos + 1);
        _receiveBuffer.erase(0, pos + 1);
        _logger->logDebug("Initial message extracted: " + message);
        if (message.find("WELCOME") != std::string::npos) {
            _logger->logInfo("WELCOME message received during initialization");
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
        _logger->logError("Timeout waiting for WELCOME message");
        return false;
    } catch (const std::exception& e) {
        _logger->logError("Error receiving initial welcome: " + std::string(e.what()));
        return false;
    }
}

int NetworkManager::receiveAndProcessData(int errorCount, int maxErrors) {
    try {
        std::string data = _connection->receive();
        if (!data.empty()) {
            _logger->logDebug("Data received (" + std::to_string(data.size()) + " bytes): " +
                        (data.size() > 20 ? data.substr(0, 20) + "..." : data));
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
            _logger->logDebug("Message extracted: " + message);
            _incomingQueue->enqueue(message);
        }
    }
}

int NetworkManager::handleReceiveError(int errorCount, int maxErrors, const std::exception& e) {
    const std::string errorMsg = e.what();
    if (errorMsg.find("Connection closed") != std::string::npos) {
        _logger->logError("Server closed connection: " + errorMsg);
        std::lock_guard<std::mutex> lock(_mutex);
        _isConnected = false;
        return maxErrors;
    } else if (errorMsg.find("Timeout") == std::string::npos) {
        errorCount++;
        _logger->logError("Error receiving data: " + errorMsg + " (error " +
                    std::to_string(errorCount) + "/" + std::to_string(maxErrors) + ")");
        if (errorCount >= maxErrors) {
            _logger->logError("Too many receive errors, disconnecting");
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
        _logger->logDebug("Message sent: " + message);
        return errorCount;
    } catch (const std::exception& e) {
        _logger->logError("Failed to send command: " + std::string(e.what()));
        errorCount++;
        if (errorCount >= maxErrors) {
            _logger->logError("Too many send errors, disconnecting");
            std::lock_guard<std::mutex> lock(_mutex);
            _isConnected = false;
        }
        return errorCount;
    }
}

int NetworkManager::handleNetworkThreadError(int errorCount, int maxErrors, const std::exception& e) {
    _logger->logError("Network thread error: " + std::string(e.what()));
    errorCount++;
    if (errorCount >= maxErrors) {
        _logger->logError("Too many network thread errors, disconnecting");
        std::lock_guard<std::mutex> lock(_mutex);
        _isConnected = false;
    }
    return errorCount;
}

void NetworkManager::processIncomingMessages() {
    std::string message;
    while (!(message = _incomingQueue->dequeue()).empty()) {
        _logger->logDebug("Processing incoming message: " + message);
        processIncomingMessage(message);
    }
}

void NetworkManager::processIncomingMessage(const std::string& message) {
    _logger->logInfo("RECV: " + message);
    try {
        handleRegularMessage(message);
    } catch (const std::exception& e) {
        handleInvalidMessage(message, e);
    }
}

void NetworkManager::handleWelcomeMessage(const std::string& message) {
    _logger->logInfo("Welcome message received: " + message);
    _logger->logInfo("Sending automatic GRAPHIC command after WELCOME");
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
            _logger->logError("Error in WELCOME callback: " + std::string(e.what()));
        }
    }
}

void NetworkManager::handleRegularMessage(const std::string& message) {
    _logger->logDebug("[NetworkManager] Attempting to parse message: " + message.substr(0, 20));
    try {
        Message parsedMessage = _protocolParser->parseMessage(message);
        _logger->logDebug("[NetworkManager] Message parsed successfully, sending to GameController");

        if (_gameController)
            _gameController->onMessageReceived(parsedMessage);
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
                _logger->logError("Error in message callback: " + std::string(e.what()));
                try {
                    localCallback("RAW", message);
                } catch (const std::exception& e2) {
                    _logger->logError("Error in raw message callback: " + std::string(e2.what()));
                }
            }
        }
    } catch (const std::exception& e) {
        _logger->logDebug("[NetworkManager] Failed to parse message: " + std::string(e.what()));
        _logger->logDebug("[NetworkManager] Problem message was: " + message);
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
                _logger->logError("Error in message callback: " + std::string(e.what()));
                try {
                    localCallback("RAW", message);
                } catch (const std::exception& e2) {
                    _logger->logError("Error in raw message callback: " + std::string(e2.what()));
                }
            }
        }
    }
}

void NetworkManager::handleInvalidMessage(const std::string& message, const std::exception& e) {
    _logger->logError("Error processing message: " + std::string(e.what()));
    MessageCallback localCallback;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        localCallback = _messageCallback;
    }
    if (localCallback) {
        try {
            localCallback("RAW", message);
        } catch (const std::exception& e) {
            _logger->logError("Error in raw message callback: " + std::string(e.what()));
        }
    }
}

void NetworkManager::setMessageCallback(MessageCallback callback) {
    _messageCallback = callback;
}

void NetworkManager::setConnectionCallback(ConnectionCallback callback) {
    _connectionCallback = callback;
}

std::shared_ptr<GameController> NetworkManager::getGameController() const {
    return _gameController;
}

void NetworkManager::setGameController(std::shared_ptr<GameController> controller) {
    _gameController = controller;
}
