/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Buffer interface implementation
*/
#pragma once

#include <string>
#include <memory>

namespace Network {

/**
 * @brief Interface pour les buffers de données
 */
class IBuffer {
public:
    virtual ~IBuffer() = default;

    virtual std::string& getData() = 0;
    virtual const std::string& getData() const = 0;
    virtual void setData(const std::string& data) = 0;
    virtual size_t size() const = 0;
    virtual void resize(size_t newSize) = 0;
    virtual void clear() = 0;
    virtual std::string toString() const = 0;
};

/**
 * @brief Factory pour créer les objets réseau
 */
class INetworkFactory {
public:
    virtual ~INetworkFactory() = default;

    virtual std::unique_ptr<ISocket> createTcpSocket() = 0;
    virtual std::unique_ptr<INetworkAddress> createAddress(const std::string& host, int port) = 0;
    virtual std::unique_ptr<IPoller> createPoller() = 0;
    virtual std::unique_ptr<IBuffer> createBuffer(size_t size = 1024) = 0;
};

} // namespace Network
