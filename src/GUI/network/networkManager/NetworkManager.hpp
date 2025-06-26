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
#include "../../shared/HeaderMessage.hpp"
#include "../networkThread/NetworkThread.hpp"
#include "../buffer/MessageQueue.hpp"
#include "../buffer/CircularBuffer.hpp"
#include "../../shared/commands/ICommand.hpp"
#include "../../shared/commands/INetworkCommandSender.hpp"

class NetworkManager : public INetworkCommandSender {
    public:
        NetworkManager();
        ~NetworkManager();

        bool connect(const std::string& host, int port);
        void disconnect();
        bool isConnected() const override;
        void sendCommand(const std::string& command) override;
        void processIncomingMessages();

        void networkThreadLoop();
        using MessageHandler = std::function<void(const Message&)>;
        void setMessageHandler(MessageHandler handler);

        using ConnectionCallback = std::function<void(bool)>;
        void setConnectionCallback(ConnectionCallback callback);

    private:
        std::unique_ptr<TcpConnection> _connection;
        std::unique_ptr<ProtocolParser> _protocolParser;
        std::unique_ptr<NetworkThread> _networkThread;
        std::unique_ptr<MessageQueue> _incomingQueue;
        std::unique_ptr<MessageQueue> _outgoingQueue;
        CircularBuffer _receiveBuffer;

        bool _isConnected;
        std::mutex _logMutex;
        mutable std::mutex _mutex;

        MessageHandler _messageHandler;
        ConnectionCallback _connectionCallback;

        bool tryReceiveInitialWelcome();
        bool processInitialWelcomeData();
        int receiveAndProcessData(int errorCount, int maxErrors);
        void extractCompleteMessages();
        int handleReceiveError(int errorCount, int maxErrors, const std::exception& e);
        int processPendingOutgoingMessages(int errorCount, int maxErrors);
        int handleNetworkThreadError(int errorCount, int maxErrors, const std::exception& e);

        void processIncomingMessage(const std::string& message);
        void handleWelcomeMessage(const std::string& message);
        void handleRegularMessage(const std::string& message);

        bool validateConnectionForSending();
        std::string formatCommand(const std::string& command);
        void queueCommandForSending(const std::string& formattedCommand);
};

#endif /* !NETWORKMANAGER_HPP_ */
