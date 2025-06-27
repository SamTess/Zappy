/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BroadcastMessageHandler
*/

#include <iostream>
#include <memory>
#include <string>
#include "BroadcastMessageHandler.hpp"

BroadcastMessageHandler::BroadcastMessageHandler(
    std::shared_ptr<GameState> gameState,
    std::shared_ptr<SoundManager> soundManager,
    std::shared_ptr<AnimationManager> animationManager)
    : _gameState(gameState), _soundManager(soundManager), _animationManager(animationManager) {}

void BroadcastMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto broadcastData = std::static_pointer_cast<BroadcastData>(data);
    int playerId = broadcastData->getPlayerId();
    const std::string& message = broadcastData->getMessage();
    if (message.empty() || playerId < 0) {
        std::cout << "[DEBUG] Skipping invalid broadcast - Player ID: " << playerId
            << ", Message: '" << message << "'" << std::endl;
        return;
    }
    if (shouldThrottleBroadcast(playerId)) {
        std::cout << "[DEBUG] Broadcast throttled for player " << playerId
            << " (too frequent)" << std::endl;
        return;
    }
    _soundManager->playBroadcastSound();
    std::cout << "[DEBUG] Broadcast triggered - Player ID: " << playerId
        << ", Message: '" << message << "'" << std::endl;
    auto playerInfo = _gameState->getPlayerInfo(playerId);
    if (playerInfo)
        _animationManager->createBroadcastEffect(playerId, _gameState);
    else
        std::cout << "[DEBUG] Player " << playerId << " not found in game state - broadcast animation skipped" << std::endl;
}

bool BroadcastMessageHandler::shouldThrottleBroadcast(int playerId) {
    auto now = std::chrono::steady_clock::now();
    auto it = _lastBroadcastTime.find(playerId);

    if (it != _lastBroadcastTime.end()) {
        auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second);
        if (timeDiff.count() < 500)
            return true;
    }
    _lastBroadcastTime[playerId] = now;
    return false;
}
