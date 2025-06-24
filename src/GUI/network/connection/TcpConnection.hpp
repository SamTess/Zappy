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
#include "../systemWrapper/NetworkAPI.hpp"

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
        std::unique_ptr<Network::ISocket> _socket;
        std::unique_ptr<Network::IBuffer> _recvBuffer;
        std::unique_ptr<Network::INetworkFactory> _networkFactory;
        size_t _initialBufferSize;

        void createAndConfigureSocket();
        void performConnect(const std::string &host, int port);
        std::string readDataFromSocket();

        class TcpConnectionException : public AException {
            public:
                explicit TcpConnectionException(const std::string &message)
                : AException("TcpConnectionException", message) {}
        };
};

#endif /* !TCPCONNECTION_HPP_ */
