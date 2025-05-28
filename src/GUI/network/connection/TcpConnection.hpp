/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tcpConnection
*/

#ifndef TCPCONNECTION_HPP_
#define TCPCONNECTION_HPP_

#include <string>
#include <netinet/in.h>
#include <poll.h>
#include "../../shared/exception/AException.hpp"

class TcpConnection {
    public:
        TcpConnection();
        ~TcpConnection();

        void connect(const std::string &host, int port);
        void send(const std::string &message);
        std::string receive();
        void close();

        bool isConnected() const;
        bool hasData() const;

    private:
        int _socket;
        struct pollfd _pollfd;
        sockaddr_in _sockaddr;

        class TcpConnectionException : public AException {
            public:
                explicit TcpConnectionException(const std::string &message)
                : AException("TcpConnectionException", message) {}
        };
};

#endif /* !TCPCONNECTION_HPP_ */
