/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** NetworkFactory
*/
#pragma once
#include <memory>
#include <string>
#include "../interfaces/INetworkFactory.hpp"
#include "../implementation/NetworkAddress.hpp"
#include "../implementation/TcpSocket.hpp"
#include "../implementation/SocketPoller.hpp"

namespace Network {
namespace Implementation {

class NetworkFactory : public INetworkFactory {
    public:
        std::unique_ptr<ISocket> createTcpSocket() override;
        std::unique_ptr<INetworkAddress> createAddress(const std::string& host, int port) override;
        std::unique_ptr<IPoller> createPoller() override;
};

std::unique_ptr<INetworkFactory> createNetworkFactory();

} // namespace implementation
} // namespace Network
