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
#include <deque>
#include <vector>
#include <map>
#include <string>
#include <utility>
#include "entities/Broadcast.hpp"
#include "entities/Tile.hpp"
#include "entities/Player.hpp"

GameState::GameState() {
    _isMapInitialized = false;
    _entityFactory = std::make_shared<EntityFactoryManager>();
}

GameState::GameState(std::shared_ptr<EntityFactoryManager> factory)
    : _entityFactory(std::move(factory)) {
    _isMapInitialized = false;
}

int GameState::getMapWidth() const {
    return _mapWidth;
}

int GameState::getMapHeight() const {
    return _mapHeight;
}

bool GameState::isMapInitialized() const {
    return _isMapInitialized;
}

std::shared_ptr<const ITile> GameState::getTile(int x, int y) const {
    if (!isValidCoordinates(x, y))
        return nullptr;
    return _tiles[y][x];
}

std::shared_ptr<ITile> GameState::getTileMutable(int x, int y) {
    if (!isValidCoordinates(x, y))
        return nullptr;
    return _tiles[y][x];
}

int GameState::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    if (!isValidCoordinates(x, y))
        return 0;
    auto tile = _tiles[y][x];
    if (!tile)
        return 0;
    return tile->getResourceQuantity(resourceType);
}

std::shared_ptr<const IPlayer> GameState::getPlayerInfo(int playerId) const {
    auto it = _players.find(playerId);

    if (it != _players.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<const IPlayerInventory> GameState::getPlayerInventory(int playerId) const {
    auto it = _inventories.find(playerId);

    if (it != _inventories.end())
        return it->second;
    return nullptr;
}

bool GameState::isPlayerOnTile(int x, int y, int playerId) const {
    if (!isValidCoordinates(x, y))
        return false;
    auto tile = _tiles[y][x];
    if (!tile)
        return false;
    const auto& playerIds = tile->getPlayerIds();
    return std::find(playerIds.begin(), playerIds.end(), playerId) != playerIds.end();
}

std::vector<int> GameState::getPlayersOnTile(int x, int y) const {
    if (!isValidCoordinates(x, y))
        return {};
    auto tile = _tiles[y][x];
    if (tile)
        return tile->getPlayerIds();
    return std::vector<int>{};
}

std::shared_ptr<const IEgg> GameState::getEggInfo(int eggId) const {
    auto it = _eggs.find(eggId);
    if (it != _eggs.end())
        return it->second;
    return nullptr;
}

std::vector<int> GameState::getEggsOnTile(int x, int y) const {
    if (!isValidCoordinates(x, y))
        return {};
    auto tile = _tiles[y][x];
    if (tile)
        return tile->getEggIds();
    return std::vector<int>{};
}

const std::vector<std::string>& GameState::getTeamNames() const {
    return _teamNames;
}

int GameState::getTimeUnit() const {
    return _timeUnit;
}

bool GameState::isGameEnded() const {
    return _gameEnded;
}

const std::string& GameState::getWinningTeam() const {
    return _winningTeam;
}

std::vector<int> GameState::getPlayerIds() const {
    std::vector<int> playerIds;

    playerIds.reserve(_players.size());
    for (const auto& pair : _players)
        playerIds.push_back(pair.first);
    return playerIds;
}

void GameState::setMapSize(int width, int height) {
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

void GameState::updateTileResources(int x, int y, int food, int linemate, int deraumere,
    int sibur, int mendiane, int phiras, int thystame) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile) {
        TileContentData tileData(x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
        tile->updateFromProtocol(tileData);
    }
}

void GameState::setTileIncantationState(int x, int y, bool isIncantating) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->setIncantating(isIncantating);
}

void GameState::addOrUpdatePlayer(const PlayerInfoData& playerData) {
    int playerId = playerData.getId();

    if (_players.find(playerId) != _players.end()) {
        const auto& oldPlayer = _players[playerId];
        int oldX = oldPlayer->getX();
        int oldY = oldPlayer->getY();
        _players[playerId]->updateFromProtocol(playerData);
        if (oldX != playerData.getX() || oldY != playerData.getY()) {
            removePlayerFromTile(playerId, oldX, oldY);
            addPlayerToTile(playerId, playerData.getX(), playerData.getY());
        }
    } else {
        if (_entityFactory)
            _players[playerId] = _entityFactory->getFactory().createPlayer(playerData);
        else
            _players[playerId] = std::make_shared<Player>(playerData);
        addPlayerToTile(playerId, playerData.getX(), playerData.getY());
    }
}

void GameState::removePlayer(int playerId) {
    auto it = _players.find(playerId);

    if (it != _players.end()) {
        const auto& player = it->second;
        removePlayerFromTile(playerId, player->getX(), player->getY());
        _players.erase(it);
        _inventories.erase(playerId);
    }
}

void GameState::updatePlayerInventory(const PlayerInventoryData& inventoryData) {
    int playerId = inventoryData.getId();
    auto it = _inventories.find(playerId);
    if (it != _inventories.end())
        it->second->updateFromProtocol(inventoryData);
    else
        _inventories[playerId] = _entityFactory->getFactory().createPlayerInventory(inventoryData);
}

void GameState::movePlayer(int playerId, int newX, int newY) {
    auto it = _players.find(playerId);

    if (it != _players.end()) {
        auto& player = it->second;
        removePlayerFromTile(playerId, player->getX(), player->getY());
        player->setPosition(newX, newY);
        addPlayerToTile(playerId, newX, newY);
    }
}

void GameState::addEgg(const EggData& eggData) {
    int eggId = eggData.getEggId();

    _eggs[eggId] = _entityFactory->getFactory().createEgg(eggData);
    addEggToTile(eggId, eggData.getX(), eggData.getY());
}

void GameState::removeEgg(int eggId) {
    auto it = _eggs.find(eggId);

    if (it != _eggs.end()) {
        const auto& egg = it->second;
        removeEggFromTile(eggId, egg->getX(), egg->getY());
        _eggs.erase(it);
    }
}

void GameState::setTeamNames(const std::vector<std::string>& teamNames) {
    _teamNames.insert(_teamNames.end(), teamNames.begin(), teamNames.end());
}

void GameState::setTimeUnit(int timeUnit) {
    _timeUnit = timeUnit;
}

void GameState::setGameEnded(bool ended, const std::string& winningTeam) {
    _gameEnded = ended;
    _winningTeam = winningTeam;
}

bool GameState::isValidCoordinates(int x, int y) const {
    if (!_isMapInitialized || x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
        return false;
    return true;
}

void GameState::addPlayerToTile(int playerId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->addPlayer(playerId);
}

void GameState::removePlayerFromTile(int playerId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->removePlayer(playerId);
}

void GameState::addEggToTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->addEgg(eggId);
}

void GameState::removeEggFromTile(int eggId, int x, int y) {
    if (!isValidCoordinates(x, y))
        return;
    auto& tile = _tiles[y][x];
    if (tile)
        tile->removeEgg(eggId);
}

const std::map<int, std::shared_ptr<IPlayer>> GameState::getPlayers() const {
    return _players;
}

const std::map<int, std::shared_ptr<IPlayer>>& GameState::getPlayers() const {
    return _players;
}

bool GameState::getSfxEnabled() const {
    return _sfxEnabled;
}

void GameState::setSfxEnabled(bool enabled) {
    _sfxEnabled = enabled;
}

float GameState::getMusicVolume() const {
    return _musicVolume;
}

void GameState::setMusicVolume(float volume) {
    _musicVolume = volume;
}
