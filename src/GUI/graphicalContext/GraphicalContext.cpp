/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicalContext
*/

#include <iostream>
#include "GraphicalContext.hpp"

GraphicalContext::GraphicalContext() {
    _updateFunctions = {
        {MessageType::MapSize, &GraphicalContext::updateMapSize},
        {MessageType::TileContent, &GraphicalContext::updateTileContent},
        {MessageType::TeamName, &GraphicalContext::updateTeamName},
        {MessageType::PlayerInfo, &GraphicalContext::updatePlayerInfo},
        {MessageType::PlayerInventory, &GraphicalContext::updatePlayerInventory},
        {MessageType::Broadcast, &GraphicalContext::updatePlayerBroadcast},
        {MessageType::Incantation, &GraphicalContext::updateIncantationStart},
        {MessageType::IncantationEnd, &GraphicalContext::updateIncantationEnd},
        {MessageType::TimeUnit, &GraphicalContext::updateTimeUnit},
        {MessageType::EndGame, &GraphicalContext::updateEndGame},
        {MessageType::ServerMessage, &GraphicalContext::updateServerMessage}
    };
}


void GraphicalContext::updateContext(const Message& message) {
    MessageType data;

    if (!message.getStructuredData()) {
        std::cerr << "Received message with no structured data" << std::endl;
        return;
    }
    data = message.getStructuredData()->getType();
    auto it = _updateFunctions.find(data);
    if (it != _updateFunctions.end()) {
        UpdateFunction updateFunc = it->second;
        (this->*updateFunc)(message.getStructuredData());
    } else {
        std::cerr << "No update function for message type: " << static_cast<int>(data) << std::endl;
    }
}

void GraphicalContext::updateMapSize(std::shared_ptr<IMessageData> data) {
    auto mapSizeData = std::static_pointer_cast<MapSizeData>(data);
    std::cout << "Map size updated to: " << mapSizeData->getWidth() << "x" << mapSizeData->getHeight() << std::endl;
}

void GraphicalContext::updateTileContent(std::shared_ptr<IMessageData> data) {
    auto tileContentData = std::static_pointer_cast<TileContentData>(data);
    std::cout << "Tile at (" << tileContentData->getX() << "," << tileContentData->getY() << ") has resources: "
              << "Food: " << tileContentData->getFood() << ", "
              << "Linemate: " << tileContentData->getLinemate() << ", "
              << "Deraumere: " << tileContentData->getDeraumere() << ", "
              << "Sibur: " << tileContentData->getSibur() << ", "
              << "Mendiane: " << tileContentData->getMendiane() << ", "
              << "Phiras: " << tileContentData->getPhiras() << ", "
              << "Thystame: " << tileContentData->getThystame() << std::endl;
}

void GraphicalContext::updateTeamName(std::shared_ptr<IMessageData> data) {
    auto teamNameData = std::static_pointer_cast<TeamNameData>(data);
    _teamNames = teamNameData->getNames();
    std::cout << "Team names updated: ";
    for (const auto& name : _teamNames) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
}

void GraphicalContext::updatePlayerInfo(std::shared_ptr<IMessageData> data) {
    auto playerInfoData = std::static_pointer_cast<PlayerInfoData>(data);
    _players[playerInfoData->getId()] = *playerInfoData;
    std::cout << "Player " << playerInfoData->getId() << " (" << playerInfoData->getTeamName() << ") at position ("
              << playerInfoData->getX() << "," << playerInfoData->getY() << ") with orientation " << playerInfoData->getOrientation()
              << " and level " << playerInfoData->getLevel() << std::endl;
}

void GraphicalContext::updatePlayerInventory(std::shared_ptr<IMessageData> data) {
    auto playerInventoryData = std::static_pointer_cast<PlayerInventoryData>(data);
    _inventories[playerInventoryData->getId()] = *playerInventoryData;
    std::cout << "Updated inventory for player " << playerInventoryData->getId() << std::endl;
}

void GraphicalContext::updatePlayerExpulsion(std::shared_ptr<IMessageData> data) {
    auto playerInfoData = std::static_pointer_cast<PlayerInfoData>(data);
    std::cout << "Player " << playerInfoData->getId() << " expelled" << std::endl;
}

void GraphicalContext::updatePlayerBroadcast(std::shared_ptr<IMessageData> data) {
    auto broadcastData = std::static_pointer_cast<BroadcastData>(data);
    std::cout << "Player " << broadcastData->getPlayerId() << " broadcast: " << broadcastData->getMessage() << std::endl;
}

void GraphicalContext::updateResourceDrop(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);
    std::cout << "Player " << resourceData->getPlayerId() << " dropped resource type " << resourceData->getResourceType() << std::endl;
}

void GraphicalContext::updateResourceCollect(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);
    std::cout << "Player " << resourceData->getPlayerId() << " collected resource type " << resourceData->getResourceType() << std::endl;
}

void GraphicalContext::updatePlayerDeath(std::shared_ptr<IMessageData> data) {
    auto playerInfoData = std::static_pointer_cast<PlayerInfoData>(data);
    std::cout << "Player " << playerInfoData->getId() << " died" << std::endl;
    _players.erase(playerInfoData->getId());
    _inventories.erase(playerInfoData->getId());
}

void GraphicalContext::updateIncantationStart(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationData>(data);
    std::cout << "Incantation started at (" << incantationData->getX() << "," << incantationData->getY() << ") for level " << incantationData->getLevel() << std::endl;
}

void GraphicalContext::updateIncantationEnd(std::shared_ptr<IMessageData> data) {
    auto incantationEndData = std::static_pointer_cast<IncantationEndData>(data);
    std::cout << "Incantation at (" << incantationEndData->getX() << "," << incantationEndData->getY() << ") ended with "
              << (incantationEndData->isSuccess() ? "success" : "failure") << std::endl;
}

void GraphicalContext::updateEggLaying(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    std::cout << "Player " << eggData->getPlayerId() << " is laying an egg" << std::endl;
}

void GraphicalContext::updateEggDrop(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    _eggs[eggData->getEggId()] = *eggData;
    std::cout << "Egg " << eggData->getEggId() << " laid by player " << eggData->getPlayerId() << " at ("
              << eggData->getX() << "," << eggData->getY() << ")" << std::endl;
}

void GraphicalContext::updateEggConnection(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    std::cout << "Player connected from egg " << eggData->getEggId() << std::endl;
    _eggs.erase(eggData->getEggId());
}

void GraphicalContext::updateEggDeath(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    std::cout << "Egg " << eggData->getEggId() << " died" << std::endl;
    _eggs.erase(eggData->getEggId());
}

void GraphicalContext::updateTimeUnit(std::shared_ptr<IMessageData> data) {
    auto timeUnitData = std::static_pointer_cast<TimeUnitData>(data);
    _timeUnit = timeUnitData->getTimeUnit();
    std::cout << "Time unit updated to: " << _timeUnit << std::endl;
}

void GraphicalContext::updateEndGame(std::shared_ptr<IMessageData> data) {
    auto endGameData = std::static_pointer_cast<EndGameData>(data);
    _gameEnded = true;
    _winningTeam = endGameData->getTeamName();
    std::cout << "Game ended. Winning team: " << _winningTeam << std::endl;
}

void GraphicalContext::updateServerMessage(std::shared_ptr<IMessageData> data) {
    auto serverMessageData = std::static_pointer_cast<ServerMessageData>(data);
    std::cout << "Server message: " << serverMessageData->getMessage() << std::endl;
}
