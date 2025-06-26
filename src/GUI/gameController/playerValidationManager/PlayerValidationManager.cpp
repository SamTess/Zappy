/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerValidationManager
*/

#include <iostream>
#include "PlayerValidationManager.hpp"
#include "../networkCommandManager/NetworkCommandManager.hpp"

PlayerValidationManager::PlayerValidationManager(std::shared_ptr<GameState> gameState,
    std::shared_ptr<NetworkCommandManager> networkManager)
    : _gameState(gameState), _networkManager(networkManager) {}

bool PlayerValidationManager::isPlayerKnown(int playerId) const {
    if (!_gameState)
        return false;
    const auto& players = _gameState->getPlayers();
    if (players.empty()) {
        std::cerr << "[PlayerValidationManager] No players known in the game state" << std::endl;
        return false;
    }
    if (players.find(playerId) == players.end()) {
        std::cerr << "[PlayerValidationManager] Player ID " << playerId << " is not known" << std::endl;
        return false;
    }
    return true;
}

bool PlayerValidationManager::validateAndRequestPlayerInfo(int playerId) {
    if (isPlayerKnown(playerId))
        return false;
    if (!_networkManager) {
        std::cerr << "[PlayerValidationManager] No network manager set - cannot request player info" << std::endl;
        return true;
    }
    _networkManager->requestPlayerPosition(playerId);
    _networkManager->requestPlayerLevel(playerId);
    _networkManager->requestPlayerInventory(playerId);
    return true;
}
