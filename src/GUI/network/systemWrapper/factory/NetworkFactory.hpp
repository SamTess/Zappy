/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Network Factory
*/
#pragma once
#include <memory>
#include <string>
#include "../interfaces/IBuffer.hpp"
#include "../implementation/NetworkAddress.hpp"
#include "../implementation/TcpSocket.hpp"
#include "../implementation/SocketPoller.hpp"
#include "../implementation/Buffer.hpp"

namespace Network {
namespace Implementation {

/**
 * @brief Factory concrète pour créer les objets réseau
 */
class NetworkFactory : public INetworkFactory {
public:
    std::unique_ptr<ISocket> createTcpSocket() override;
    std::unique_ptr<INetworkAddress> createAddress(const std::string& host, int port) override;
    std::unique_ptr<IPoller> createPoller() override;
    std::unique_ptr<IBuffer> createBuffer(size_t size = 1024) override;
};

/**
 * @brief Fonction utilitaire pour obtenir la factory
 */
std::unique_ptr<INetworkFactory> createNetworkFactory();

} // namespace implementation
} // namespace Network
