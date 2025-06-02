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
        std::shared_ptr<TcpConnection> _connection;
        std::shared_ptr<ProtocolParser> _protocolParser;
        std::shared_ptr<NetworkThread> _networkThread;
        std::shared_ptr<MessageQueue> _incomingQueue;
        std::shared_ptr<CircularBuffer> _receiveBuffer;

        std::atomic<bool> _isConnected;
        mutable std::mutex _mutex;
        std::condition_variable _cv;

        MessageCallback _messageCallback;
        ConnectionCallback _connectionCallback;

        void networkThreadLoop();
        void processIncomingMessage(const std::string& message);
};

#endif /* !NETWORKMANAGER_HPP_ */
