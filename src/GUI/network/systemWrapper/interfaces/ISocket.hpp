/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Socket interface implementation
*/
#pragma once

#include <string>
#include <memory>

namespace Network {

/**
 * @brief Interface abstraite pour les sockets
 */
class ISocket {
public:
    virtual ~ISocket() = default;

    virtual bool connect(const std::string& host, int port) = 0;
    virtual bool isConnected() const = 0;
    virtual void disconnect() = 0;
    virtual bool send(const std::string& data) = 0;
    virtual std::string receive() = 0;
    virtual bool hasData() const = 0;
    virtual bool setNonBlocking() = 0;
};

/**
 * @brief Interface pour les adresses réseau
 */
class INetworkAddress {
public:
    virtual ~INetworkAddress() = default;

    virtual std::string getHost() const = 0;
    virtual int getPort() const = 0;
    virtual std::string toString() const = 0;
};

/**
 * @brief Interface pour le polling de sockets
 */
class IPoller {
public:
    virtual ~IPoller() = default;

    virtual void addSocket(int fd, int events) = 0;
    virtual void removeSocket(int fd) = 0;
    virtual int poll(int timeoutMs) = 0;
    virtual bool hasEvent(int fd, int event) const = 0;
    virtual void clear() = 0;
};

} // namespace Network
