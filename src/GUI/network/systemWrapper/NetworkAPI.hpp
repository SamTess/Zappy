/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** NetworkAPI
*/

#pragma once

#include <memory>
#include "interfaces/ISocket.hpp"
#include "factory/NetworkFactory.hpp"

namespace Network {

inline std::unique_ptr<INetworkFactory> createFactory() {
    return Implementation::createNetworkFactory();
}

} // namespace Network
