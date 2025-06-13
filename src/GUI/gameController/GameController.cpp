/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameController implementation
*/

#include "GameController.hpp"
#include <iostream>
#include <algorithm>

GameController::GameController() : _gameState(std::make_shared<GameState>()) {
}

void GameController::onMessageReceived(const Message& message) {
    processMessage(message);
}

// a faire autrement map surement
void GameController::processMessage(const Message& message) {
    if (!message.getStructuredData())
        return;
    MessageType messageType = message.getStructuredData()->getType();

    switch (messageType) {
        case MessageType::MapSize:
            handleMapSize(message.getStructuredData());
            break;
        case MessageType::TileContent:
            handleTileContent(message.getStructuredData());
            break;
        case MessageType::TeamName:
            handleTeamName(message.getStructuredData());
            break;
        case MessageType::PlayerInfo:
            handlePlayerInfo(message.getStructuredData());
            break;
        case MessageType::PlayerInventory:
            handlePlayerInventory(message.getStructuredData());
            break;
        case MessageType::Broadcast:
            handlePlayerBroadcast(message.getStructuredData());
            break;
        case MessageType::Incantation:
            handleIncantationStart(message.getStructuredData());
            break;
        case MessageType::IncantationEnd:
            handleIncantationEnd(message.getStructuredData());
            break;
        case MessageType::TimeUnit:
            handleTimeUnit(message.getStructuredData());
            break;
        case MessageType::EndGame:
            handleEndGame(message.getStructuredData());
            break;
        case MessageType::ServerMessage:
            handleServerMessage(message.getStructuredData());
            break;
        case MessageType::Egg:
            handleEggDrop(message.getStructuredData());
            break;
        default:
            std::cerr << "[GameController] No handler for message type: " << static_cast<int>(messageType) << std::endl;
            break;
    }
}

void GameController::handleMapSize(std::shared_ptr<IMessageData> data) {
    auto mapSizeData = std::static_pointer_cast<MapSizeData>(data);

    _gameState->setMapSize(mapSizeData->getWidth(), mapSizeData->getHeight());
}

void GameController::handleTileContent(std::shared_ptr<IMessageData> data) {
    auto tileData = std::static_pointer_cast<TileContentData>(data);
    int x = tileData->getX();
    int y = tileData->getY();

    _gameState->updateTileResources(x, y,
        tileData->getFood(), tileData->getLinemate(), tileData->getDeraumere(),
        tileData->getSibur(), tileData->getMendiane(), tileData->getPhiras(), tileData->getThystame());
}

void GameController::handleTeamName(std::shared_ptr<IMessageData> data) {
    auto teamData = std::static_pointer_cast<TeamNameData>(data);
    _gameState->setTeamNames(teamData->getNames());
}

void GameController::handlePlayerInfo(std::shared_ptr<IMessageData> data) {
    auto playerData = std::static_pointer_cast<PlayerInfoData>(data);
    int playerId = playerData->getId();
    auto existingPlayer = _gameState->getPlayerInfo(playerId);
    if (existingPlayer) {
        int oldX = existingPlayer->getX();
        int oldY = existingPlayer->getY();
        int newX = playerData->getX();
        int newY = playerData->getY();
        if (oldX != newX || oldY != newY) {
            _gameState->addOrUpdatePlayer(*playerData);
            return;
        }
    }
    _gameState->addOrUpdatePlayer(*playerData);
}

void GameController::handlePlayerInventory(std::shared_ptr<IMessageData> data) {
    auto inventoryData = std::static_pointer_cast<PlayerInventoryData>(data);

    _gameState->updatePlayerInventory(*inventoryData);
}

void GameController::handlePlayerExpulsion(std::shared_ptr<IMessageData> data) {
    auto playerData = std::static_pointer_cast<PlayerInfoData>(data);
}

void GameController::handlePlayerBroadcast(std::shared_ptr<IMessageData> data) {
    auto broadcastData = std::static_pointer_cast<BroadcastData>(data);
}

void GameController::handleResourceDrop(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);
}

void GameController::handleResourceCollect(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);
}

void GameController::handlePlayerDeath(std::shared_ptr<IMessageData> data) {
    auto playerData = std::static_pointer_cast<PlayerInfoData>(data);
    int playerId = playerData->getId();

    _gameState->removePlayer(playerId);
}

void GameController::handleIncantationStart(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationData>(data);
    int x = incantationData->getX();
    int y = incantationData->getY();
    // int level = incantationData->getLevel();

    _gameState->setTileIncantationState(x, y, true);
}

void GameController::handleIncantationEnd(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationEndData>(data);
    int x = incantationData->getX();
    int y = incantationData->getY();
    // bool success = incantationData->isSuccess();

    _gameState->setTileIncantationState(x, y, false);
}

void GameController::handleEggLaying(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
}

void GameController::handleEggDrop(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);

    switch (eggData->getAction()) {
        case EggData::EggAction::Drop: {
            // int eggId = eggData->getEggId();
            // int x = eggData->getX();
            // int y = eggData->getY();
            _gameState->addEgg(*eggData);
            break;
        }
        case EggData::EggAction::Connection:
            handleEggConnection(data);
            break;
        case EggData::EggAction::Death:
            handleEggDeath(data);
            break;
        case EggData::EggAction::Laying:
            handleEggLaying(data);
            break;
    }
}

void GameController::handleEggConnection(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    int eggId = eggData->getEggId();

    auto existingEgg = _gameState->getEggInfo(eggId);
    if (existingEgg)
        _gameState->removeEgg(eggId);
}

void GameController::handleEggDeath(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    int eggId = eggData->getEggId();
    auto existingEgg = _gameState->getEggInfo(eggId);

    if (existingEgg)
        _gameState->removeEgg(eggId);
}

void GameController::handleTimeUnit(std::shared_ptr<IMessageData> data) {
    auto timeData = std::static_pointer_cast<TimeUnitData>(data);
    _gameState->setTimeUnit(timeData->getTimeUnit());
}

void GameController::handleEndGame(std::shared_ptr<IMessageData> data) {
    auto endGameData = std::static_pointer_cast<EndGameData>(data);
    _gameState->setGameEnded(true, endGameData->getTeamName());
}

void GameController::handleServerMessage(std::shared_ptr<IMessageData> data) {
    auto serverData = std::static_pointer_cast<ServerMessageData>(data);
}
