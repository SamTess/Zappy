/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicalContext
*/

#include <iostream>
#include <algorithm>
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
        {MessageType::ServerMessage, &GraphicalContext::updateServerMessage},
        {MessageType::Egg, &GraphicalContext::updateEggDrop}
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
    _mapWidth = mapSizeData->getWidth();
    _mapHeight = mapSizeData->getHeight();
    _mapTiles.resize(_mapHeight, std::vector<TileData>(_mapWidth));
    std::cout << "Map size updated to: " << _mapWidth << "x" << _mapHeight << std::endl;
    notifyMapSizeChanged();
}

void GraphicalContext::updateTileContent(std::shared_ptr<IMessageData> data) {
    auto tileContentData = std::static_pointer_cast<TileContentData>(data);
    int x = tileContentData->getX();
    int y = tileContentData->getY();
    if (!isValidCoordinates(x, y)) {
        std::cerr << "Invalid coordinates for tile content update: (" << x << "," << y << ")" << std::endl;
        return;
    }
    _mapTiles[y][x].resources[static_cast<int>(ResourceType::FOOD)] = tileContentData->getFood();
    _mapTiles[y][x].resources[static_cast<int>(ResourceType::LINEMATE)] = tileContentData->getLinemate();
    _mapTiles[y][x].resources[static_cast<int>(ResourceType::DERAUMERE)] = tileContentData->getDeraumere();
    _mapTiles[y][x].resources[static_cast<int>(ResourceType::SIBUR)] = tileContentData->getSibur();
    _mapTiles[y][x].resources[static_cast<int>(ResourceType::MENDIANE)] = tileContentData->getMendiane();
    _mapTiles[y][x].resources[static_cast<int>(ResourceType::PHIRAS)] = tileContentData->getPhiras();
    _mapTiles[y][x].resources[static_cast<int>(ResourceType::THYSTAME)] = tileContentData->getThystame();
    std::cout << "Tile at (" << x << "," << y << ") has resources: "
              << "Food: " << tileContentData->getFood() << ", "
              << "Linemate: " << tileContentData->getLinemate() << ", "
              << "Deraumere: " << tileContentData->getDeraumere() << ", "
              << "Sibur: " << tileContentData->getSibur() << ", "
              << "Mendiane: " << tileContentData->getMendiane() << ", "
              << "Phiras: " << tileContentData->getPhiras() << ", "
              << "Thystame: " << tileContentData->getThystame() << std::endl;
    notifyTileChanged(x, y);
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
    if (_players.find(playerInfoData->getId()) != _players.end()) {
        const auto& oldPlayer = _players[playerInfoData->getId()];
        int oldX = oldPlayer.getX();
        int oldY = oldPlayer.getY();
        if (oldX != playerInfoData->getX() || oldY != playerInfoData->getY()) {
            if (isValidCoordinates(oldX, oldY)) {
                auto& oldPlayerIds = _mapTiles[oldY][oldX].playerIds;
                oldPlayerIds.erase(std::remove(oldPlayerIds.begin(), oldPlayerIds.end(), playerInfoData->getId()), oldPlayerIds.end());
                notifyTileChanged(oldX, oldY);
            }
        }
    }
    _players[playerInfoData->getId()] = *playerInfoData;
    int x = playerInfoData->getX();
    int y = playerInfoData->getY();
    if (isValidCoordinates(x, y)) {
        auto& playerIds = _mapTiles[y][x].playerIds;
        if (std::find(playerIds.begin(), playerIds.end(), playerInfoData->getId()) == playerIds.end()) {
            playerIds.push_back(playerInfoData->getId());
            notifyTileChanged(x, y);
        }
    }
    std::cout << "Player " << playerInfoData->getId() << " (" << playerInfoData->getTeamName() << ") at position ("
              << x << "," << y << ") with orientation " << playerInfoData->getOrientation()
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
    if (_players.find(playerInfoData->getId()) != _players.end()) {
        const auto& player = _players[playerInfoData->getId()];
        int x = player.getX();
        int y = player.getY();
        if (isValidCoordinates(x, y)) {
            auto& playerIds = _mapTiles[y][x].playerIds;
            playerIds.erase(std::remove(playerIds.begin(), playerIds.end(), playerInfoData->getId()), playerIds.end());
            notifyTileChanged(x, y);
        }
    }
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
    
    switch (eggData->getAction()) {
        case EggData::EggAction::Drop: {
            _eggs[eggData->getEggId()] = *eggData;
            int x = eggData->getX();
            int y = eggData->getY();
            if (isValidCoordinates(x, y)) {
                auto& eggIds = _mapTiles[y][x].eggIds;
                if (std::find(eggIds.begin(), eggIds.end(), eggData->getEggId()) == eggIds.end()) {
                    eggIds.push_back(eggData->getEggId());
                    notifyTileChanged(x, y);
                }
            }
            std::cout << "Egg " << eggData->getEggId() << " laid by player " << eggData->getPlayerId() << " at ("
                      << x << "," << y << ")" << std::endl;
            break;
        }
        case EggData::EggAction::Connection:
            updateEggConnection(data);
            break;
        case EggData::EggAction::Death:
            updateEggDeath(data);
            break;
        case EggData::EggAction::Laying:
            std::cout << "Player " << eggData->getPlayerId() << " is laying an egg" << std::endl;
            break;
    }
}

void GraphicalContext::updateEggConnection(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    if (_eggs.find(eggData->getEggId()) != _eggs.end()) {
        const auto& egg = _eggs[eggData->getEggId()];
        int x = egg.getX();
        int y = egg.getY();
        if (isValidCoordinates(x, y)) {
            auto& eggIds = _mapTiles[y][x].eggIds;
            eggIds.erase(std::remove(eggIds.begin(), eggIds.end(), eggData->getEggId()), eggIds.end());
            notifyTileChanged(x, y);
        }
    }
    std::cout << "Player connected from egg " << eggData->getEggId() << std::endl;
    _eggs.erase(eggData->getEggId());
}

void GraphicalContext::updateEggDeath(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    if (_eggs.find(eggData->getEggId()) != _eggs.end()) {
        const auto& egg = _eggs[eggData->getEggId()];
        int x = egg.getX();
        int y = egg.getY();
        if (isValidCoordinates(x, y)) {
            auto& eggIds = _mapTiles[y][x].eggIds;
            eggIds.erase(std::remove(eggIds.begin(), eggIds.end(), eggData->getEggId()), eggIds.end());
            notifyTileChanged(x, y);
        }
    }
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

void GraphicalContext::addObserver(std::shared_ptr<IGraphicalContextObserver> observer) {
    auto it = std::find_if(_observers.begin(), _observers.end(),
        [&](const std::shared_ptr<IGraphicalContextObserver>& obs) {
            return obs == observer;
        });
    if (it == _observers.end()) {
        _observers.push_back(observer);
        if (_mapWidth > 0 && _mapHeight > 0) {
            observer->onMapSizeChanged(_mapWidth, _mapHeight);
            for (int y = 0; y < _mapHeight; ++y) {
                for (int x = 0; x < _mapWidth; ++x) {
                    observer->onTileChanged(x, y, _mapTiles[y][x]);
                }
            }
        }
    }
}

void GraphicalContext::removeObserver(std::shared_ptr<IGraphicalContextObserver> observer) {
    _observers.erase(
        std::remove_if(_observers.begin(), _observers.end(),
            [&](const std::shared_ptr<IGraphicalContextObserver>& obs) {
                return obs == observer;
            }
        ),
        _observers.end()
    );
}

void GraphicalContext::notifyMapSizeChanged() {
    for (const auto& observer : _observers) {
        observer->onMapSizeChanged(_mapWidth, _mapHeight);
    }
}

void GraphicalContext::notifyTileChanged(int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    for (const auto& observer : _observers) {
        observer->onTileChanged(x, y, _mapTiles[y][x]);
    }
}

bool GraphicalContext::isValidCoordinates(int x, int y) const {
    return (x >= 0 && x < _mapWidth && y >= 0 && y < _mapHeight);
}

ResourceType GraphicalContext::getDominantResourceType(int x, int y) const {
    if (!isValidCoordinates(x, y))
        return ResourceType::COUNT;
    const TileData& tile = _mapTiles[y][x];
    ResourceType dominantType = ResourceType::COUNT;
    int maxQuantity = 0;
    for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
        if (tile.resources[i] > maxQuantity) {
            maxQuantity = tile.resources[i];
            dominantType = static_cast<ResourceType>(i);
        }
    }
    if (maxQuantity == 0)
        return ResourceType::COUNT;
    return dominantType;
}

const TileData& GraphicalContext::getTileData(int x, int y) const {
    static TileData emptyTile;
    if (!isValidCoordinates(x, y))
        return emptyTile;
    return _mapTiles[y][x];
}

int GraphicalContext::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    if (!isValidCoordinates(x, y) || resourceType == ResourceType::COUNT)
        return 0;
    return _mapTiles[y][x].resources[static_cast<int>(resourceType)];
}

bool GraphicalContext::isPlayerOnTile(int x, int y, int playerId) const {
    if (!isValidCoordinates(x, y))
        return false;
    const auto& playerIds = _mapTiles[y][x].playerIds;
    return std::find(playerIds.begin(), playerIds.end(), playerId) != playerIds.end();
}

const std::shared_ptr<PlayerInfoData> GraphicalContext::getPlayerInfo(int playerId) const {
    return (_players.find(playerId) != _players.end()) ?
        std::make_shared<PlayerInfoData>(_players.at(playerId)) : nullptr;
}