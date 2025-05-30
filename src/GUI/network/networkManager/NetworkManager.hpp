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

class NetworkObserver;

class NetworkManager {
    public:
        NetworkManager();
        ~NetworkManager();

        bool connect(const std::string& host, int port);
        void disconnect();
        bool isConnected() const;
        void sendCommand(const std::string& command);
        void processIncomingMessages();
        void addObserver(NetworkObserver* observer);
        void removeObserver(NetworkObserver* observer);

    private:
        std::shared_ptr<TcpConnection> _connection;
        std::shared_ptr<ProtocolParser> _protocolParser;
        std::shared_ptr<NetworkThread> _networkThread;
        std::shared_ptr<MessageQueue> _incomingQueue;
        std::shared_ptr<CircularBuffer> _receiveBuffer;

        std::atomic<bool> _isConnected;
        mutable std::mutex _mutex;
        std::condition_variable _cv;

        std::vector<NetworkObserver*> _observers; // Utilisation de pointeurs bruts : la durée de vie des observers est gérée à l'extérieur du NetworkManager (pattern observer). On ne doit pas prendre la propriété ni gérer la destruction ici.
        mutable std::mutex _observersMutex;

        void networkThreadLoop();
        void processIncomingMessage(const std::string& message);
        void notifyObservers(const std::string& header, const std::string& data);
};

// Interface Observer pour recevoir des notifications
class NetworkObserver {
    public:
        virtual ~NetworkObserver() = default;
        virtual void onMessage(const std::string& header, const std::string& data) = 0;
        virtual void onConnectionStatusChanged(bool connected) = 0;
};

#endif /* !NETWORKMANAGER_HPP_ */
