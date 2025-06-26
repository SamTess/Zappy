/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** NetworkFactory
*/

#include <string>
#include <memory>
#include "NetworkFactory.hpp"

namespace Network {
namespace Implementation {

std::unique_ptr<ISocket> NetworkFactory::createTcpSocket() {
    return std::make_unique<TcpSocket>();
}

std::unique_ptr<INetworkAddress> NetworkFactory::createAddress(const std::string& host, int port) {
    return std::make_unique<NetworkAddress>(host, port);
}

std::unique_ptr<IPoller> NetworkFactory::createPoller() {
    return std::make_unique<SocketPoller>();
}

std::unique_ptr<IBuffer> NetworkFactory::createBuffer(size_t size) {
    return std::make_unique<Buffer>(size);
}

std::unique_ptr<INetworkFactory> createNetworkFactory() {
    return std::make_unique<NetworkFactory>();
}

} // namespace implementation
} // namespace Network
