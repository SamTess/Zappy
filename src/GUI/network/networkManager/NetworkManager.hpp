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
        void networkThreadLoop();

    private:
        std::unique_ptr<TcpConnection> _connection;
        std::unique_ptr<ProtocolParser> _protocolParser;
        std::unique_ptr<NetworkThread> _networkThread;
        std::unique_ptr<MessageQueue> _incomingQueue;
        std::unique_ptr<MessageQueue> _outgoingQueue;
        std::string _receiveBuffer;
        std::unique_ptr<GraphicalContext> _graphicalContext;

        bool _isConnected;
        std::mutex _logMutex;
        mutable std::mutex _mutex;
        std::condition_variable _cv;

        MessageCallback _messageCallback;
        ConnectionCallback _connectionCallback;

        // Méthodes de gestion du thread réseau
        bool tryReceiveInitialWelcome();
        bool processInitialWelcomeData();
        int receiveAndProcessData(int errorCount, int maxErrors);
        void processReceivedData();
        void extractCompleteMessages();
        int handleReceiveError(int errorCount, int maxErrors, const std::exception& e);
        int processPendingOutgoingMessages(int errorCount, int maxErrors);
        int handleNetworkThreadError(int errorCount, int maxErrors, const std::exception& e);

        void processIncomingMessage(const std::string& message);
        void handleWelcomeMessage(const std::string& message);
        void handleRegularMessage(const std::string& message);
        void handleInvalidMessage(const std::string& message, const std::exception& e);

        bool validateConnectionForSending();
        std::string formatCommand(const std::string& command);
        void logOutgoingCommand(const std::string& formattedCommand);
        void queueCommandForSending(const std::string& formattedCommand);
};

#endif /* !NETWORKMANAGER_HPP_ */
