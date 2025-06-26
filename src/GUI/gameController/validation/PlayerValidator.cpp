/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerValidator - Implémentation de la validation des joueurs
*/

#include <memory>
#include <iostream>
#include "PlayerValidator.hpp"

PlayerValidator::PlayerValidator(std::shared_ptr<GameState> gameState,
    std::shared_ptr<NetworkCommandManager> networkManager)
    : _gameState(gameState), _networkManager(networkManager) {
}

bool PlayerValidator::isPlayerKnown(int playerId) {
    const auto &players = _gameState->getPlayers();
    return !players.empty() && players.find(playerId) != players.end();
}

bool PlayerValidator::validateAndRequestPlayerData(int playerId) {
    if (isPlayerKnown(playerId))
        return true;
    if (!_networkManager) {
        std::cerr << "[PlayerValidator] No network manager set - cannot request player info" << std::endl;
        return false;
    }
    _networkManager->requestPlayerInfo(playerId);
    return false;
}
