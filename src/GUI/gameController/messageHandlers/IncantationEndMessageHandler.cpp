/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IncantationEndMessageHandler
*/

#include "IncantationEndMessageHandler.hpp"

IncantationEndMessageHandler::IncantationEndMessageHandler(std::shared_ptr<GameState> gameState)
    : _gameState(gameState) {}

void IncantationEndMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto incantationEndData = std::static_pointer_cast<IncantationEndData>(data);
    int x = incantationEndData->getX();
    int y = incantationEndData->getY();
    _gameState->setTileIncantationState(x, y, false);
}
