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
private:
    std::string _host;
    int _port;
    sockaddr_in _addr;
    void resolveAddress();

public:
    NetworkAddress(const std::string& host, int port);

    std::string getHost() const override;
    int getPort() const override;
    std::string toString() const override;
    const sockaddr* getSockAddr() const;
    socklen_t getAddrSize() const;
};

} // namespace implementation
} // namespace Network
