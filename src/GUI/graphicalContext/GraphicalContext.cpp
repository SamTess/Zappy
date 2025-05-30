/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicalContext
*/

#include "GraphicalContext.hpp"

GraphicalContext::GraphicalContext()
{
    _updateFunctions = {
        {MessageType::MapSize, [this](std::shared_ptr<IMessageData> data)
            { updateMapSize(std::static_pointer_cast<MapSizeData>(data)); }},
        {MessageType::TileContent, [this](std::shared_ptr<IMessageData> data)
            { updateTileContent(std::static_pointer_cast<TileContentData>(data)); }},
        {MessageType::TeamName, [this](std::shared_ptr<IMessageData> data)
            { updateTeamName(std::static_pointer_cast<TeamNameData>(data)); }},
        {MessageType::PlayerInfo, [this](std::shared_ptr<IMessageData> data)
            { updatePlayerInfo(std::static_pointer_cast<PlayerInfoData>(data)); }},
        {MessageType::PlayerInventory, [this](std::shared_ptr<IMessageData> data)
            { updatePlayerInventory(std::static_pointer_cast<PlayerInventoryData>(data)); }},
        {MessageType::Broadcast, [this](std::shared_ptr<IMessageData> data)
            { updatePlayerBroadcast(std::static_pointer_cast<BroadcastData>(data)); }},
        {MessageType::Incantation, [this](std::shared_ptr<IMessageData> data)
            { updateIncantationStart(std::static_pointer_cast<IncantationData>(data)); }},
        {MessageType::IncantationEnd, [this](std::shared_ptr<IMessageData> data)
            { updateIncantationEnd(std::static_pointer_cast<IncantationEndData>(data)); }},
        {MessageType::TimeUnit, [this](std::shared_ptr<IMessageData> data)
            { updateTimeUnit(std::static_pointer_cast<TimeUnitData>(data)); }},
        {MessageType::EndGame, [this](std::shared_ptr<IMessageData> data)
            { updateEndGame(std::static_pointer_cast<EndGameData>(data)); }},
        {MessageType::ServerMessage, [this](std::shared_ptr<IMessageData> data)
            { updateServerMessage(std::static_pointer_cast<ServerMessageData>(data)); }}
    };
}


void GraphicalContext::updateContext(const Message& message)
{
    MessageType data;

    if (!message.getStructuredData()) {
        std::cerr << "Received message with no structured data" << std::endl;
        return;
    }
    data = message.getStructuredData()->getType();
    auto it = _updateFunctions.find(data);
    if (it != _updateFunctions.end()) {
        it->second(message.getStructuredData());
    } else {
        std::cerr << "No update function for message type: " << static_cast<int>(data) << std::endl;
    }
}

void GraphicalContext::updateMapSize(std::shared_ptr<MapSizeData> data)
{
    std::cout << "Map size updated to: " << data->getWidth() << "x" << data->getHeight() << std::endl;
}

void GraphicalContext::updateTileContent(std::shared_ptr<TileContentData> data)
{
    std::cout << "Tile at (" << data->getX() << "," << data->getY() << ") has resources: "
              << "Food: " << data->getFood() << ", "
              << "Linemate: " << data->getLinemate() << ", "
              << "Deraumere: " << data->getDeraumere() << ", "
              << "Sibur: " << data->getSibur() << ", "
              << "Mendiane: " << data->getMendiane() << ", "
              << "Phiras: " << data->getPhiras() << ", "
              << "Thystame: " << data->getThystame() << std::endl;
}

void GraphicalContext::updateTeamName(std::shared_ptr<TeamNameData> data)
{
    _teamNames = data->getNames();
    std::cout << "Team names updated: ";
    for (const auto& name : _teamNames) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
}

void GraphicalContext::updatePlayerInfo(std::shared_ptr<PlayerInfoData> data)
{
    _players[data->getId()] = *data;
    std::cout << "Player " << data->getId() << " (" << data->getTeamName() << ") at position ("
              << data->getX() << "," << data->getY() << ") with orientation " << data->getOrientation()
              << " and level " << data->getLevel() << std::endl;
}

void GraphicalContext::updatePlayerInventory(std::shared_ptr<PlayerInventoryData> data)
{
    _inventories[data->getId()] = *data;
    std::cout << "Updated inventory for player " << data->getId() << std::endl;
}

void GraphicalContext::updatePlayerExpulsion(std::shared_ptr<PlayerInfoData> data)
{
    std::cout << "Player " << data->getId() << " expelled" << std::endl;
}

void GraphicalContext::updatePlayerBroadcast(std::shared_ptr<BroadcastData> data)
{
    std::cout << "Player " << data->getPlayerId() << " broadcast: " << data->getMessage() << std::endl;
}

void GraphicalContext::updateResourceDrop(std::shared_ptr<ResourceData> data)
{
    std::cout << "Player " << data->getPlayerId() << " dropped resource type " << data->getResourceType() << std::endl;
}

void GraphicalContext::updateResourceCollect(std::shared_ptr<ResourceData> data)
{
    std::cout << "Player " << data->getPlayerId() << " collected resource type " << data->getResourceType() << std::endl;
}

void GraphicalContext::updatePlayerDeath(std::shared_ptr<PlayerInfoData> data)
{
    std::cout << "Player " << data->getId() << " died" << std::endl;
    _players.erase(data->getId());
    _inventories.erase(data->getId());
}

void GraphicalContext::updateIncantationStart(std::shared_ptr<IncantationData> data)
{
    std::cout << "Incantation started at (" << data->getX() << "," << data->getY() << ") for level " << data->getLevel() << std::endl;
}

void GraphicalContext::updateIncantationEnd(std::shared_ptr<IncantationEndData> data)
{
    std::cout << "Incantation at (" << data->getX() << "," << data->getY() << ") ended with "
              << (data->isSuccess() ? "success" : "failure") << std::endl;
}

void GraphicalContext::updateEggLaying(std::shared_ptr<EggData> data)
{
    std::cout << "Player " << data->getPlayerId() << " is laying an egg" << std::endl;
}

void GraphicalContext::updateEggDrop(std::shared_ptr<EggData> data)
{
    _eggs[data->getEggId()] = *data;
    std::cout << "Egg " << data->getEggId() << " laid by player " << data->getPlayerId() << " at ("
              << data->getX() << "," << data->getY() << ")" << std::endl;
}

void GraphicalContext::updateEggConnection(std::shared_ptr<EggData> data)
{
    std::cout << "Player connected from egg " << data->getEggId() << std::endl;
    _eggs.erase(data->getEggId());
}

void GraphicalContext::updateEggDeath(std::shared_ptr<EggData> data)
{
    std::cout << "Egg " << data->getEggId() << " died" << std::endl;
    _eggs.erase(data->getEggId());
}

void GraphicalContext::updateTimeUnit(std::shared_ptr<TimeUnitData> data)
{
    _timeUnit = data->getTimeUnit();
    std::cout << "Time unit updated to: " << _timeUnit << std::endl;
}

void GraphicalContext::updateEndGame(std::shared_ptr<EndGameData> data)
{
    _gameEnded = true;
    _winningTeam = data->getTeamName();
    std::cout << "Game ended. Winning team: " << _winningTeam << std::endl;
}

void GraphicalContext::updateServerMessage(std::shared_ptr<ServerMessageData> data)
{
    std::cout << "Server message: " << data->getMessage() << std::endl;
}
