/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Network API implementation
*/

#pragma once

/**
 * @brief Nouvelle API réseau encapsulée pour Zappy
 *
 * Cette API remplace SystemWrapper en offrant :
 * - Encapsulation complète des détails système
 * - Abstraction via des interfaces
 * - Séparation des responsabilités
 * - Facilité de test et de maintenance
 */

#include <memory>
#include "interfaces/ISocket.hpp"
#include "interfaces/IBuffer.hpp"
#include "factory/NetworkFactory.hpp"

namespace Network {

/**
 * @brief Point d'entrée principal de l'API réseau
 *
 * Usage:
 * auto factory = Network::createFactory();
 * auto socket = factory->createTcpSocket();
 * socket->connect("localhost", 8080);
 */
inline std::unique_ptr<INetworkFactory> createFactory() {
    return Implementation::createNetworkFactory();
}

} // namespace Network
