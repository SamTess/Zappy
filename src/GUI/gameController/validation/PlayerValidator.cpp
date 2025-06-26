/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerValidator - Implémentation de la validation des joueurs
*/

#include "PlayerValidator.hpp"
#include <iostream>

PlayerValidator::PlayerValidator(std::shared_ptr<GameState> gameState,
                               std::shared_ptr<NetworkCommandManager> networkManager)
    : _gameState(gameState), _networkManager(networkManager) {
}

bool PlayerValidator::isPlayerKnown(int playerId) {
    const auto &players = _gameState->getPlayers();
    if (players.empty()) {
        std::cerr << "[PlayerValidator] No players known in the game state" << std::endl;
        return false;
    }
    if (players.find(playerId) == players.end()) {
        std::cerr << "[PlayerValidator] Player ID " << playerId << " is not known" << std::endl;
        return false;
    }
    return true;
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
