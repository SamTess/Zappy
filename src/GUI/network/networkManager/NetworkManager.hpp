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

    private:
        std::unique_ptr<TcpConnection> _connection;
        std::unique_ptr<ProtocolParser> _protocolParser;
        std::unique_ptr<NetworkThread> _networkThread;
        std::unique_ptr<MessageQueue> _incomingQueue;
        std::unique_ptr<MessageQueue> _outgoingQueue;
        std::unique_ptr<CircularBuffer> _receiveBuffer;
        std::unique_ptr<GraphicalContext> _graphicalContext;

        bool _isConnected;
        std::mutex _logMutex;

        void startNetworkThread();
        void stopNetworkThread();
        void networkThreadLoop();
        void processIncomingMessage(const std::string& message);
        void processOutgoingMessages();
};

#endif /* !NETWORKMANAGER_HPP_ */
