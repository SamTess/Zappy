/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** INetworkFactory
*/

#pragma once

#include <string>
#include <memory>
#include "ISocket.hpp"

namespace Network {

class INetworkFactory {
    public:
        virtual ~INetworkFactory() = default;

        virtual std::unique_ptr<ISocket> createTcpSocket() = 0;
        virtual std::unique_ptr<INetworkAddress> createAddress(const std::string& host, int port) = 0;
        virtual std::unique_ptr<IPoller> createPoller() = 0;
};

} // namespace Network
