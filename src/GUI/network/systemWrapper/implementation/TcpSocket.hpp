/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** TcpSocket implementation
*/
#pragma once

#include <memory>
#include <string>
#include "../interfaces/ISocket.hpp"
#include "NetworkAddress.hpp"

namespace Network {
namespace Implementation {

/**
 * @brief Implémentation concrète d'un socket TCP
 */
class TcpSocket : public ISocket {
private:
    int _fd;
    bool _connected;
    std::unique_ptr<NetworkAddress> _address;

    void createSocket();
    void performConnect();
    void cleanup();
    bool isValidDescriptor() const;

public:
    TcpSocket();
    ~TcpSocket() override;

    bool connect(const std::string& host, int port) override;
    bool isConnected() const override;
    void disconnect() override;
    bool send(const std::string& data) override;
    std::string receive() override;
    bool hasData() const override;
    bool setNonBlocking() override;
    int getFileDescriptor() const;
};

} // namespace implementation
} // namespace Network
