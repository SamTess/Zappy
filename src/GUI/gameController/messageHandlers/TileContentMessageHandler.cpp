/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TileContentMessageHandler
*/

#include <memory>
#include "TileContentMessageHandler.hpp"

TileContentMessageHandler::TileContentMessageHandler(std::shared_ptr<GameState> gameState,
    std::shared_ptr<NetworkCommandManager> networkManager)
    : _gameState(gameState), _networkManager(networkManager) {}

void TileContentMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    if (!_gameState->isMapInitialized()) {
        handleUninitializedMap();
        return;
    }
    
    auto tileData = std::static_pointer_cast<TileContentData>(data);
    updateTileContent(tileData);
}

void TileContentMessageHandler::handleUninitializedMap() {
    if (_networkManager) {
        _networkManager->requestMapSize();
        _networkManager->requestMapContent();
    }
}

void TileContentMessageHandler::updateTileContent(std::shared_ptr<TileContentData> tileData) {
    int x = tileData->getX();
    int y = tileData->getY();
    _gameState->updateTileResources(x, y,
        tileData->getFood(), tileData->getLinemate(), tileData->getDeraumere(),
        tileData->getSibur(), tileData->getMendiane(), tileData->getPhiras(),
        tileData->getThystame());
}
