/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerExpulsionMessageHandler
*/

#include <memory>
#include "PlayerExpulsionMessageHandler.hpp"

PlayerExpulsionMessageHandler::PlayerExpulsionMessageHandler(
    std::shared_ptr<GameState> gameState,
    std::shared_ptr<PlayerValidationManager> validator,
    std::shared_ptr<AnimationManager> animationManager)
    : _gameState(gameState), _validator(validator), _animationManager(animationManager) {}

void PlayerExpulsionMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto expulsionData = std::static_pointer_cast<PlayerExpulsionData>(data);
    int playerId = expulsionData->getPlayerId();
    if (_validator->validateAndRequestPlayerInfo(playerId))
        return;
    auto playerInfo = _gameState->getPlayerInfo(playerId);
    if (!playerInfo)
        return;
    int mapWidth = _gameState->getMapWidth();
    int mapHeight = _gameState->getMapHeight();
    ZappyTypes::Vector3 currentPos = Zappy::EjectionAnimationManager::getInstance()
        .convertTileToWorldPosition(playerInfo->getX(), playerInfo->getY(), mapWidth, mapHeight);
    ZappyTypes::Vector3 ejectionDirection = calculateEjectionDirection(playerInfo->getOrientation());
    int destTileX = playerInfo->getX() + static_cast<int>(ejectionDirection.x);
    int destTileY = playerInfo->getY() + static_cast<int>(ejectionDirection.z);
    ZappyTypes::Vector3 destinationPos = Zappy::EjectionAnimationManager::getInstance()
        .convertTileToWorldPosition(destTileX, destTileY, mapWidth, mapHeight);
    _animationManager->startEjectionAnimation(playerId, currentPos, destinationPos);
}

ZappyTypes::Vector3 PlayerExpulsionMessageHandler::calculateEjectionDirection(int orientation) {
    switch (orientation) {
        case 1:
            return {0.0f, 0.0f, -1.0f};
        case 2:
            return {1.0f, 0.0f, 0.0f};
        case 3:
            return {0.0f, 0.0f, 1.0f};
        case 4:
            return {-1.0f, 0.0f, 0.0f};
        default:
            return {1.0f, 0.0f, 0.0f};
    }
}
