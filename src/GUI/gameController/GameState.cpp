/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameState implementation
*/

#include "GameState.hpp"
#include <iostream>
#include <algorithm>

GameState::GameState() {
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

const TileData& GameState::getTileData(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y)) {
        static TileData emptyTile;
        return emptyTile;
    }
    return _mapTiles[y][x];
}

int GameState::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y) || static_cast<int>(resourceType) >= static_cast<int>(ResourceType::COUNT))
        return 0;
    return _mapTiles[y][x].resources[static_cast<int>(resourceType)];
}

ResourceType GameState::getDominantResourceType(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return ResourceType::FOOD;

    const auto& resources = _mapTiles[y][x].resources;
    int maxQuantity = 0;
    ResourceType dominantType = ResourceType::FOOD;
    for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
        if (resources[i] > maxQuantity) {
            maxQuantity = resources[i];
            dominantType = static_cast<ResourceType>(i);
        }
    }
    return dominantType;
}

std::shared_ptr<const PlayerInfoData> GameState::getPlayerInfo(int playerId) const {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _players.find(playerId);

    return (it != _players.end()) ? std::make_shared<PlayerInfoData>(it->second) : nullptr;
}

std::shared_ptr<const PlayerInventoryData> GameState::getPlayerInventory(int playerId) const {
    std::lock_guard<std::mutex> lock(_mutex);

    auto it = _inventories.find(playerId);
    return (it != _inventories.end()) ? std::make_shared<PlayerInventoryData>(it->second) : nullptr;
}

bool GameState::isPlayerOnTile(int x, int y, int playerId) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return false;
    const auto& playerIds = _mapTiles[y][x].playerIds;
    return std::find(playerIds.begin(), playerIds.end(), playerId) != playerIds.end();
}

std::vector<int> GameState::getPlayersOnTile(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!isValidCoordinates(x, y))
        return {};
    return _mapTiles[y][x].playerIds;
}

std::shared_ptr<const EggData> GameState::getEggInfo(int eggId) const {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _eggs.find(eggId);

    return (it != _eggs.end()) ? std::make_shared<EggData>(it->second) : nullptr;
}

std::vector<int> GameState::getEggsOnTile(int x, int y) const {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return {};
    return _mapTiles[y][x].eggIds;
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
    _mapTiles.resize(_mapHeight, std::vector<TileData>(_mapWidth));
    for (int y = 0; y < _mapHeight; ++y) {
        for (int x = 0; x < _mapWidth; ++x) {
            _mapTiles[y][x] = TileData{};
        }
    }
    _isMapInitialized = true;
}

void GameState::updateTileResources(int x, int y, int food, int linemate, int deraumere,
    int sibur, int mendiane, int phiras, int thystame) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _mapTiles[y][x];
    tile.resources[static_cast<int>(ResourceType::FOOD)] = food;
    tile.resources[static_cast<int>(ResourceType::LINEMATE)] = linemate;
    tile.resources[static_cast<int>(ResourceType::DERAUMERE)] = deraumere;
    tile.resources[static_cast<int>(ResourceType::SIBUR)] = sibur;
    tile.resources[static_cast<int>(ResourceType::MENDIANE)] = mendiane;
    tile.resources[static_cast<int>(ResourceType::PHIRAS)] = phiras;
    tile.resources[static_cast<int>(ResourceType::THYSTAME)] = thystame;
}

void GameState::setTileIncantationState(int x, int y, bool isIncantating) {
    std::lock_guard<std::mutex> lock(_mutex);

    if (!isValidCoordinates(x, y))
        return;
    _mapTiles[y][x].isIncantating = isIncantating;
}

void GameState::addOrUpdatePlayer(const PlayerInfoData& playerData) {
    std::lock_guard<std::mutex> lock(_mutex);
    int playerId = playerData.getId();

    if (_players.find(playerId) != _players.end()) {
        const auto& oldPlayer = _players[playerId];
        removePlayerFromTile(playerId, oldPlayer.getX(), oldPlayer.getY());
    }
    _players[playerId] = playerData;
    addPlayerToTile(playerId, playerData.getX(), playerData.getY());
}

void GameState::removePlayer(int playerId) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _players.find(playerId);

    if (it != _players.end()) {
        const auto& player = it->second;
        removePlayerFromTile(playerId, player.getX(), player.getY());
        _players.erase(it);
        _inventories.erase(playerId);
    }
}

void GameState::updatePlayerInventory(const PlayerInventoryData& inventoryData) {
    std::lock_guard<std::mutex> lock(_mutex);
    _inventories[inventoryData.getId()] = inventoryData;
}

void GameState::movePlayer(int playerId, int newX, int newY) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _players.find(playerId);

    if (it != _players.end()) {
        auto& player = it->second;
        removePlayerFromTile(playerId, player.getX(), player.getY());
        addPlayerToTile(playerId, newX, newY);
    }
}

void GameState::addEgg(const EggData& eggData) {
    std::lock_guard<std::mutex> lock(_mutex);
    int eggId = eggData.getEggId();

    _eggs[eggId] = eggData;
    addEggToTile(eggId, eggData.getX(), eggData.getY());
}

void GameState::removeEgg(int eggId) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _eggs.find(eggId);

    if (it != _eggs.end()) {
        const auto& egg = it->second;
        removeEggFromTile(eggId, egg.getX(), egg.getY());
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
    auto& playerIds = _mapTiles[y][x].playerIds;
    if (std::find(playerIds.begin(), playerIds.end(), playerId) == playerIds.end()) {
        playerIds.push_back(playerId);
    }
}

void GameState::removePlayerFromTile(int playerId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& playerIds = _mapTiles[y][x].playerIds;
    playerIds.erase(std::remove(playerIds.begin(), playerIds.end(), playerId), playerIds.end());
}

void GameState::addEggToTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& eggIds = _mapTiles[y][x].eggIds;
    if (std::find(eggIds.begin(), eggIds.end(), eggId) == eggIds.end()) {
        eggIds.push_back(eggId);
    }
}

void GameState::removeEggFromTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& eggIds = _mapTiles[y][x].eggIds;
    eggIds.erase(std::remove(eggIds.begin(), eggIds.end(), eggId), eggIds.end());
}
