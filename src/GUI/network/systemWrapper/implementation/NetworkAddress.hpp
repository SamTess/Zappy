/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** NetworkAddress
*/

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include "../interfaces/ISocket.hpp"

namespace Network {
namespace Implementation {

class NetworkAddress : public INetworkAddress {
    public:
        NetworkAddress(const std::string& host, int port);

        std::string getHost() const override;
        int getPort() const override;
        std::string toString() const override;
        const sockaddr* getSockAddr() const;
        socklen_t getAddrSize() const;

    private:
        std::string _host;
        sockaddr_in _addr;
        int _port;
        void resolveAddress();
};

} // namespace implementation
} // namespace Network
