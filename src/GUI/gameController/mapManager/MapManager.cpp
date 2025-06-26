/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MapManager
*/

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>
#include "MapManager.hpp"

MapManager::MapManager()
    : _mapWidth(0), _mapHeight(0), _isMapInitialized(false) {
    _entityFactory = std::make_shared<EntityFactoryManager>();
}

MapManager::MapManager(std::shared_ptr<EntityFactoryManager> factory)
    : _mapWidth(0), _mapHeight(0), _isMapInitialized(false), _entityFactory(std::move(factory)) {
}

int MapManager::getMapWidth() const {
    return _mapWidth;
}

int MapManager::getMapHeight() const {
    return _mapHeight;
}

bool MapManager::isMapInitialized() const {
    return _isMapInitialized;
}

void MapManager::setMapSize(int width, int height) {
    if (_isMapInitialized)
        return;
    _mapWidth = width;
    _mapHeight = height;
    _tiles.resize(_mapHeight, std::vector<std::shared_ptr<ITile>>(_mapWidth));
    for (int y = 0; y < _mapHeight; ++y) {
        for (int x = 0; x < _mapWidth; ++x) {
            if (_entityFactory)
                _tiles[y][x] = _entityFactory->getFactory().createTile(x, y);
            else
                _tiles[y][x] = std::make_shared<Tile>(x, y);
        }
    }
    _isMapInitialized = true;
}

std::shared_ptr<const ITile> MapManager::getTile(int x, int y) const {
    if (!isValidCoordinates(x, y))
        return nullptr;
    return _tiles[y][x];
}

std::shared_ptr<ITile> MapManager::getTileMutable(int x, int y) {
    if (!isValidCoordinates(x, y))
        return nullptr;
    return _tiles[y][x];
}

int MapManager::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    if (!isValidCoordinates(x, y))
        return 0;
    auto tile = _tiles[y][x];
    if (!tile)
        return 0;
    return tile->getResourceQuantity(resourceType);
}

void MapManager::updateTileResources(int x, int y, int food, int linemate, int deraumere,
                                   int sibur, int mendiane, int phiras, int thystame) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        TileContentData tileData(x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
        tile->updateFromProtocol(tileData);
    }
}

void MapManager::setTileIncantationState(int x, int y, bool isIncantating) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->setIncantating(isIncantating);
}

std::vector<int> MapManager::getPlayersOnTile(int x, int y) const {
    if (!isValidCoordinates(x, y))
        return {};
    auto tile = _tiles[y][x];
    if (tile)
        return tile->getPlayerIds();
    return std::vector<int>{};
}

bool MapManager::isPlayerOnTile(int x, int y, int playerId) const {
    if (!isValidCoordinates(x, y))
        return false;
    auto tile = _tiles[y][x];
    if (!tile)
        return false;
    const auto& playerIds = tile->getPlayerIds();
    return std::find(playerIds.begin(), playerIds.end(), playerId) != playerIds.end();
}

void MapManager::addPlayerToTile(int playerId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->addPlayer(playerId);
}

void MapManager::removePlayerFromTile(int playerId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->removePlayer(playerId);
}

std::vector<int> MapManager::getEggsOnTile(int x, int y) const {
    if (!isValidCoordinates(x, y))
        return {};
    auto tile = _tiles[y][x];
    if (tile)
        return tile->getEggIds();
    return std::vector<int>{};
}

void MapManager::addEggToTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->addEgg(eggId);
}

void MapManager::removeEggFromTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->removeEgg(eggId);
}

bool MapManager::isValidCoordinates(int x, int y) const {
    if (!_isMapInitialized || x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
        return false;
    return true;
}
