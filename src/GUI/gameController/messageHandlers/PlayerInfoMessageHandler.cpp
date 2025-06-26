/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInfoMessageHandler
*/

#include <memory>
#include "PlayerInfoMessageHandler.hpp"
#include "../../renderer/EjectionAnimationManager.hpp"

PlayerInfoMessageHandler::PlayerInfoMessageHandler(
    std::shared_ptr<GameState> gameState,
    std::shared_ptr<PlayerValidationManager> validator,
    std::shared_ptr<AnimationManager> animationManager,
    std::shared_ptr<SoundManager> soundManager)
    : _gameState(gameState), _validator(validator),
      _animationManager(animationManager), _soundManager(soundManager) {}

void PlayerInfoMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto playerData = std::static_pointer_cast<PlayerInfoData>(data);
    int playerId = playerData->getId();
    auto existingPlayer = _gameState->getPlayerInfo(playerId);

    if (handlePlayerValidation(playerData, existingPlayer))
        return;

    handleMissingTeamInfo(playerData, existingPlayer);
    if (handleExistingPlayerWithValidOrientation(playerData, existingPlayer, playerId))
        return;
    handleIncompleteOrientationData(playerData, existingPlayer);
    updatePlayerInGameState(playerData);
}

bool PlayerInfoMessageHandler::handlePlayerValidation(std::shared_ptr<PlayerInfoData> playerData,
                                                      std::shared_ptr<const IPlayer> existingPlayer) {
    if (!existingPlayer && playerData->getTeamName().empty()) {
        return _validator->validateAndRequestPlayerInfo(playerData->getId());
    }
    return false;
}

bool PlayerInfoMessageHandler::handleExistingPlayerWithValidOrientation(std::shared_ptr<PlayerInfoData> playerData,
                                                                         std::shared_ptr<const IPlayer> existingPlayer,
                                                                         int playerId) {
    if (existingPlayer && playerData->getOrientation() != -1) {
        if (handlePlayerDeath(playerData, existingPlayer, playerId))
            return true;
        if (handlePlayerMovement(playerData, existingPlayer))
            return true;
    }
    return false;
}

void PlayerInfoMessageHandler::updatePlayerInGameState(std::shared_ptr<PlayerInfoData> playerData) {
    _gameState->addOrUpdatePlayer(*playerData);
}

void PlayerInfoMessageHandler::handleMissingTeamInfo(std::shared_ptr<PlayerInfoData> playerData,
                                                     std::shared_ptr<const IPlayer> existingPlayer) {
    if (existingPlayer && playerData->getTeamName().empty()) {
        playerData->setTeamName(existingPlayer->getTeamName());
    }
}

bool PlayerInfoMessageHandler::handlePlayerDeath(std::shared_ptr<PlayerInfoData> playerData,
                                                  std::shared_ptr<const IPlayer> existingPlayer,
                                                  int playerId) {
    if (!playerData->isAlive()) {
        processPlayerDeath(existingPlayer, playerId);
        return true;
    }
    return false;
}

void PlayerInfoMessageHandler::processPlayerDeath(std::shared_ptr<const IPlayer> existingPlayer, int playerId) {
    ZappyTypes::Vector3 playerWorldPos = Zappy::EjectionAnimationManager::getInstance()
        .convertTileToWorldPosition(existingPlayer->getX(), existingPlayer->getY(),
                                  _gameState->getMapWidth(), _gameState->getMapHeight());
    _animationManager->startDeathAnimation(playerId, playerWorldPos, existingPlayer->getTeamName());
    _gameState->removePlayer(playerId);

    if (_soundManager->isSfxEnabled()) {
        _soundManager->playDeathSound();
    }
}

bool PlayerInfoMessageHandler::handlePlayerMovement(std::shared_ptr<PlayerInfoData> playerData,
                                                     std::shared_ptr<const IPlayer> existingPlayer) {
    int oldX = existingPlayer->getX();
    int oldY = existingPlayer->getY();
    int newX = playerData->getX();
    int newY = playerData->getY();

    if (oldX != newX || oldY != newY) {
        _gameState->addOrUpdatePlayer(*playerData);
        return true;
    }
    return false;
}

void PlayerInfoMessageHandler::handleIncompleteOrientationData(std::shared_ptr<PlayerInfoData> playerData,
                                                                std::shared_ptr<const IPlayer> existingPlayer) {
    if (playerData->getOrientation() == -1 && existingPlayer) {
        playerData->setTeamName(existingPlayer->getTeamName());
        playerData->setX(existingPlayer->getX());
        playerData->setY(existingPlayer->getY());
        playerData->setOrientation(existingPlayer->getOrientation());
    }
}
