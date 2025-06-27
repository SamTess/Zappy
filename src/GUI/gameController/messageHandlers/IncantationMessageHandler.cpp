/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IncantationMessageHandler
*/

#include <memory>
#include "IncantationMessageHandler.hpp"

IncantationMessageHandler::IncantationMessageHandler(std::shared_ptr<GameState> gameState,
    std::shared_ptr<PlayerValidationManager> validator)
    : _gameState(gameState), _validator(validator) {}

void IncantationMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationData>(data);
    int x = incantationData->getX();
    int y = incantationData->getY();

    for (int playerId : incantationData->getPlayerIds())
        _validator->validateAndRequestPlayerInfo(playerId);
    _gameState->setTileIncantationState(x, y, true);
}
