/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tcpConnection
*/

#ifndef TCPCONNECTION_HPP_
#define TCPCONNECTION_HPP_

#include <string>
#include <memory>
#include "../../shared/exception/AException.hpp"
#include "../utils/SystemWrapper.hpp"

class TcpConnection {
    public:
        explicit TcpConnection(size_t initialBufferSize = 4096);
        ~TcpConnection();

        void connect(const std::string &host, int port);
        void send(const std::string &message);
        std::string receive();
        void close();

        bool isConnected() const;
        bool hasData() const;

    private:
        int _socket;
        std::unique_ptr<SystemWrapper::SafePollFd> _pollfd;
        std::unique_ptr<SystemWrapper::SafeSockAddr> _sockaddr;
        std::unique_ptr<SystemWrapper::SafeBuffer> _recvBuffer;
        size_t _initialBufferSize;

        void createAndConfigureSocket();
        void resolveAddress(const std::string &host, int port);
        void performConnect();
        void waitForConnection(int timeoutMs = 5000);
        bool waitForReadData(int timeoutMs);
        std::string readDataFromSocket();
        void ensureBufferSize();
        void adjustBufferSize(ssize_t bytesReceived);

        class TcpConnectionException : public AException {
            public:
                explicit TcpConnectionException(const std::string &message)
                : AException("TcpConnectionException", message) {}
        };
};

#endif /* !TCPCONNECTION_HPP_ */
