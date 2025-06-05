/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** networkManager
*/

#ifndef NETWORKMANAGER_HPP_
    #define NETWORKMANAGER_HPP_

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "../connection/TcpConnection.hpp"
#include "../protocol/ProtocolParser.hpp"
#include "../protocol/HeaderMessage.hpp"
#include "../networkThread/NetworkThread.hpp"
#include "../buffer/MessageQueue.hpp"
#include "../buffer/CircularBuffer.hpp"
#include "../../graphicalContext/GraphicalContext.hpp"

class NetworkManager {
    public:
        NetworkManager();
        ~NetworkManager();

        bool connect(const std::string& host, int port);
        void disconnect();
        bool isConnected() const;
        void sendCommand(const std::string& command);
        void processIncomingMessages();

        // Callback pour récupérer les messages
        using MessageCallback = std::function<void(const std::string&, const std::string&)>;
        void setMessageCallback(MessageCallback callback);
        // Callback pour les changements de statut de connexion
        using ConnectionCallback = std::function<void(bool)>;
        void setConnectionCallback(ConnectionCallback callback);

    private:
        std::unique_ptr<TcpConnection> _connection;
        std::unique_ptr<ProtocolParser> _protocolParser;
        std::unique_ptr<NetworkThread> _networkThread;
        std::unique_ptr<MessageQueue> _incomingQueue;
        std::unique_ptr<MessageQueue> _outgoingQueue;
        std::shared_ptr<CircularBuffer> _receiveBuffer;
        std::unique_ptr<GraphicalContext> _graphicalContext;

        bool _isConnected;
        std::mutex _logMutex;
        mutable std::mutex _mutex;
        std::condition_variable _cv;

        MessageCallback _messageCallback;
        ConnectionCallback _connectionCallback;

        // Méthodes de gestion du thread réseau
        void networkThreadLoop();
        void tryReceiveInitialWelcome(std::shared_ptr<bool> welcomeReceived);
        int receiveAndProcessData(int errorCount, int maxErrors, std::shared_ptr<bool> welcomeReceived);
        void processReceivedData(std::shared_ptr<bool> welcomeReceived);
        int handleReceiveError(int errorCount, int maxErrors, const std::exception& e);
        int processPendingOutgoingMessages(int errorCount, int maxErrors);
        int handleNetworkThreadError(int errorCount, int maxErrors, const std::exception& e);

        // Méthodes de traitement des messages entrants
        void processIncomingMessage(const std::string& message);
        void handleWelcomeMessage(const std::string& message);
        void handleRegularMessage(const std::string& message);
        void executeMessageCallback(const std::string& message);
        void handleInvalidMessage(const std::string& message, const std::exception& e);

        // Méthodes pour l'envoi des commandes
        bool validateConnectionForSending();
        std::string formatCommand(const std::string& command);
        void logOutgoingCommand(const std::string& formattedCommand);
        void queueCommandForSending(const std::string& formattedCommand);
};

#endif /* !NETWORKMANAGER_HPP_ */
