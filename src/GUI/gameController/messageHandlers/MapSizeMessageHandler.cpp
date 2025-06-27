/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MapSizeMessageHandler
*/

#include <memory>
#include "MapSizeMessageHandler.hpp"

MapSizeMessageHandler::MapSizeMessageHandler(std::shared_ptr<GameState> gameState)
    : _gameState(gameState) {}

void MapSizeMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto mapSizeData = std::static_pointer_cast<MapSizeData>(data);
    _gameState->setMapSize(mapSizeData->getWidth(), mapSizeData->getHeight());
}
