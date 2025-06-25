/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** NetworkAddress implementation
*/

#include "NetworkAddress.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <stdexcept>

namespace Network {
namespace Implementation {

NetworkAddress::NetworkAddress(const std::string& host, int port)
    : _host(host), _port(port) {
    resolveAddress();
}

void NetworkAddress::resolveAddress() {
    struct hostent* server = nullptr;

    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    if (inet_pton(AF_INET, _host.c_str(), &_addr.sin_addr) <= 0) {
        server = gethostbyname(_host.c_str());
        if (server == nullptr)
            throw std::runtime_error("Unable to resolve host: " + _host);
        std::memcpy(&_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    }
}

std::string NetworkAddress::getHost() const {
    return _host;
}

int NetworkAddress::getPort() const {
    return _port;
}

std::string NetworkAddress::toString() const {
    return _host + ":" + std::to_string(_port);
}

const sockaddr* NetworkAddress::getSockAddr() const {
    return reinterpret_cast<const sockaddr*>(&_addr);
}

socklen_t NetworkAddress::getAddrSize() const {
    return sizeof(_addr);
}

} // namespace implementation
} // namespace Network
