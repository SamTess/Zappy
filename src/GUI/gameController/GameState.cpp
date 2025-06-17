/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameState implementation
*/

#include "GameState.hpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <utility>

GameState::GameState() {
    _entityFactory = std::make_shared<EntityFactoryManager>();
}

GameState::GameState(std::shared_ptr<EntityFactoryManager> factory)
    : _entityFactory(std::move(factory)) {
}

int GameState::getMapWidth() const {
    std::lock_guard<std::mutex> lock(_mutex);

    return _mapWidth;
}

int GameState::getMapHeight() const {
    std::lock_guard<std::mutex> lock(_mutex);

    return _mapHeight;
}

bool GameState::isMapInitialized() const {
    std::lock_guard<std::mutex> lock(_mutex);

    return _isMapInitialized;
}

std::shared_ptr<const ITile> GameState::getTile(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return nullptr;
    return _tiles[y][x];
}

std::shared_ptr<ITile> GameState::getTileMutable(int x, int y) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return nullptr;
    return _tiles[y][x];
}

int GameState::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return 0;
    auto tile = _tiles[y][x];
    return tile ? tile->getResourceQuantity(resourceType) : 0;
}

ResourceType GameState::getDominantResourceType(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return ResourceType::FOOD;

    auto tile = _tiles[y][x];
    return tile ? tile->getDominantResourceType() : ResourceType::FOOD;
}

std::shared_ptr<const IPlayer> GameState::getPlayerInfo(int playerId) const {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _players.find(playerId);

    return (it != _players.end()) ? it->second : nullptr;
}

std::shared_ptr<const IPlayerInventory> GameState::getPlayerInventory(int playerId) const {
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _inventories.find(playerId);
    return (it != _inventories.end()) ? it->second : nullptr;
}

bool GameState::isPlayerOnTile(int x, int y, int playerId) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return false;
    auto tile = _tiles[y][x];
    if (!tile)
        return false;
    const auto& playerIds = tile->getPlayerIds();
    return std::find(playerIds.begin(), playerIds.end(), playerId) != playerIds.end();
}

std::vector<int> GameState::getPlayersOnTile(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!isValidCoordinates(x, y))
        return {};
    auto tile = _tiles[y][x];
    return tile ? tile->getPlayerIds() : std::vector<int>{};
}

std::shared_ptr<const IEgg> GameState::getEggInfo(int eggId) const {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _eggs.find(eggId);

    return (it != _eggs.end()) ? it->second : nullptr;
}

std::vector<int> GameState::getEggsOnTile(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return {};
    auto tile = _tiles[y][x];
    return tile ? tile->getEggIds() : std::vector<int>{};
}

const std::vector<std::string>& GameState::getTeamNames() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _teamNames;
}

int GameState::getTimeUnit() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _timeUnit;
}

bool GameState::isGameEnded() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _gameEnded;
}

const std::string& GameState::getWinningTeam() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _winningTeam;
}


void GameState::setMapSize(int width, int height) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (_isMapInitialized)
        return;
    _mapWidth = width;
    _mapHeight = height;
    _tiles.resize(_mapHeight, std::vector<std::shared_ptr<ITile>>(_mapWidth));
    for (int y = 0; y < _mapHeight; ++y) {
        for (int x = 0; x < _mapWidth; ++x) {
            _tiles[y][x] = _entityFactory->getFactory().createTile(x, y);
        }
    }
    _isMapInitialized = true;
}

void GameState::updateTileResources(int x, int y, int food, int linemate, int deraumere,
    int sibur, int mendiane, int phiras, int thystame) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        TileContentData tileData(x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
        tile->updateFromProtocol(tileData);
    }
}

void GameState::setTileIncantationState(int x, int y, bool isIncantating) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        tile->setIncantating(isIncantating);
    }
}

void GameState::addOrUpdatePlayer(const PlayerInfoData& playerData) {
    std::lock_guard<std::mutex> lock(_mutex);
    int playerId = playerData.getId();

    if (_players.find(playerId) != _players.end()) {
        const auto& oldPlayer = _players[playerId];
        _players[playerId]->updateFromProtocol(playerData);
        if (oldPlayer->getX() != playerData.getX() || oldPlayer->getY() != playerData.getY()) {
            removePlayerFromTile(playerId, oldPlayer->getX(), oldPlayer->getY());
            addPlayerToTile(playerId, playerData.getX(), playerData.getY());
        }
    } else {
        _players[playerId] = _entityFactory->getFactory().createPlayer(playerData);
        addPlayerToTile(playerId, playerData.getX(), playerData.getY());
    }
}

void GameState::removePlayer(int playerId) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _players.find(playerId);

    if (it != _players.end()) {
        const auto& player = it->second;
        removePlayerFromTile(playerId, player->getX(), player->getY());
        _players.erase(it);
        _inventories.erase(playerId);
    }
}

void GameState::updatePlayerInventory(const PlayerInventoryData& inventoryData) {
    std::lock_guard<std::mutex> lock(_mutex);
    int playerId = inventoryData.getId();
    auto it = _inventories.find(playerId);
    if (it != _inventories.end()) {
        it->second->updateFromProtocol(inventoryData);
    } else {
        _inventories[playerId] = _entityFactory->getFactory().createPlayerInventory(inventoryData);
    }
}

void GameState::movePlayer(int playerId, int newX, int newY) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _players.find(playerId);

    if (it != _players.end()) {
        auto& player = it->second;
        removePlayerFromTile(playerId, player->getX(), player->getY());
        player->setPosition(newX, newY);
        addPlayerToTile(playerId, newX, newY);
    }
}

void GameState::addEgg(const EggData& eggData) {
    std::lock_guard<std::mutex> lock(_mutex);
    int eggId = eggData.getEggId();

    _eggs[eggId] = _entityFactory->getFactory().createEgg(eggData);
    addEggToTile(eggId, eggData.getX(), eggData.getY());
}

void GameState::removeEgg(int eggId) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _eggs.find(eggId);

    if (it != _eggs.end()) {
        const auto& egg = it->second;
        removeEggFromTile(eggId, egg->getX(), egg->getY());
        _eggs.erase(it);
    }
}

void GameState::setTeamNames(const std::vector<std::string>& teamNames) {
    std::lock_guard<std::mutex> lock(_mutex);
    _teamNames = teamNames;
}

void GameState::setTimeUnit(int timeUnit) {
    std::lock_guard<std::mutex> lock(_mutex);
    _timeUnit = timeUnit;
}

void GameState::setGameEnded(bool ended, const std::string& winningTeam) {
    std::lock_guard<std::mutex> lock(_mutex);
    _gameEnded = ended;
    _winningTeam = winningTeam;
}

bool GameState::isValidCoordinates(int x, int y) const {
    return (x >= 0 && x < _mapWidth && y >= 0 && y < _mapHeight);
}

void GameState::addPlayerToTile(int playerId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        tile->addPlayer(playerId);
    }
}

void GameState::removePlayerFromTile(int playerId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        tile->removePlayer(playerId);
    }
}

void GameState::addEggToTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        tile->addEgg(eggId);
    }
}

void GameState::removeEggFromTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        tile->removeEgg(eggId);
    }
}
