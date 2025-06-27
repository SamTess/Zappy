/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** SocketPoller
*/

#pragma once

#include <poll.h>
#include <vector>
#include <unordered_map>
#include "../interfaces/ISocket.hpp"
#include "TcpSocket.hpp"

namespace Network {
namespace Implementation {

class SocketPoller : public IPoller {
    public:
        SocketPoller();
        void addSocket(int fd, int events) override;
        void removeSocket(int fd) override;
        int poll(int timeoutMs) override;
        bool hasEvent(int fd, int event) const override;
        void clear() override;
        void addSocket(const TcpSocket& socket, int events);
        void removeSocket(const TcpSocket& socket);
        bool hasEvent(const TcpSocket& socket, int event) const;

    private:
        std::vector<pollfd> _pollfds;
        std::unordered_map<int, size_t> _fdToIndex;

        void updateIndices();
};

} // namespace implementation
} // namespace Network
