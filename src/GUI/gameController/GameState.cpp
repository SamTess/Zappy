/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameState
*/

#include <utility>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "GameState.hpp"

GameState::GameState() {
    _gameStateManager = std::make_shared<GameStateManager>();
    _adapter = std::make_unique<GameStateAdapter>(_gameStateManager);
}

GameState::GameState(std::shared_ptr<EntityFactoryManager> factory) {
    _gameStateManager = std::make_shared<GameStateManager>(std::move(factory));
    _adapter = std::make_unique<GameStateAdapter>(_gameStateManager);
}

int GameState::getMapWidth() const {
    return _adapter->getMapWidth();
}

int GameState::getMapHeight() const {
    return _adapter->getMapHeight();
}

bool GameState::isMapInitialized() const {
    return _adapter->isMapInitialized();
}

std::shared_ptr<const ITile> GameState::getTile(int x, int y) const {
    return _adapter->getTile(x, y);
}

std::shared_ptr<ITile> GameState::getTileMutable(int x, int y) {
    return _adapter->getTileMutable(x, y);
}

int GameState::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    return _adapter->getResourceQuantity(x, y, resourceType);
}

void GameState::setMapSize(int width, int height) {
    _adapter->setMapSize(width, height);
}

void GameState::updateTileResources(int x, int y, int food, int linemate, int deraumere,
                                     int sibur, int mendiane, int phiras, int thystame) {
    _adapter->updateTileResources(x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
}

void GameState::setTileIncantationState(int x, int y, bool isIncantating) {
    _adapter->setTileIncantationState(x, y, isIncantating);
}

bool GameState::isPlayerOnTile(int x, int y, int playerId) const {
    return _adapter->isPlayerOnTile(x, y, playerId);
}

std::vector<int> GameState::getPlayersOnTile(int x, int y) const {
    return _adapter->getPlayersOnTile(x, y);
}

std::vector<int> GameState::getEggsOnTile(int x, int y) const {
    return _adapter->getEggsOnTile(x, y);
}

std::shared_ptr<const IPlayer> GameState::getPlayerInfo(int playerId) const {
    return _adapter->getPlayerInfo(playerId);
}

std::shared_ptr<const IPlayerInventory> GameState::getPlayerInventory(int playerId) const {
    return _adapter->getPlayerInventory(playerId);
}

std::vector<int> GameState::getPlayerIds() const {
    return _adapter->getPlayerIds();
}

const std::map<int, std::shared_ptr<IPlayer>> GameState::getPlayers() const {
    return _adapter->getPlayers();
}

void GameState::addOrUpdatePlayer(const PlayerInfoData& playerData) {
    _adapter->addOrUpdatePlayer(playerData);
}

void GameState::removePlayer(int playerId) {
    _adapter->removePlayer(playerId);
}

void GameState::updatePlayerInventory(const PlayerInventoryData& inventoryData) {
    _adapter->updatePlayerInventory(inventoryData);
}

void GameState::movePlayer(int playerId, int newX, int newY) {
    _adapter->movePlayer(playerId, newX, newY);
}

std::shared_ptr<const IEgg> GameState::getEggInfo(int eggId) const {
    return _adapter->getEggInfo(eggId);
}

void GameState::addEgg(const EggData& eggData) {
    _adapter->addEgg(eggData);
}

void GameState::removeEgg(int eggId) {
    _adapter->removeEgg(eggId);
}

const std::vector<std::string>& GameState::getTeamNames() const {
    return _adapter->getTeamNames();
}

void GameState::setTeamNames(const std::vector<std::string>& teamNames) {
    _adapter->setTeamNames(teamNames);
}

int GameState::getTimeUnit() const {
    return _adapter->getTimeUnit();
}

void GameState::setTimeUnit(int timeUnit) {
    _adapter->setTimeUnit(timeUnit);
}

bool GameState::isGameEnded() const {
    return _adapter->isGameEnded();
}

const std::string& GameState::getWinningTeam() const {
    return _adapter->getWinningTeam();
}

void GameState::setGameEnded(bool ended, const std::string& winningTeam) {
    _adapter->setGameEnded(ended, winningTeam);
}

bool GameState::getSfxEnabled() const {
    return _adapter->getSfxEnabled();
}

void GameState::setSfxEnabled(bool enabled) {
    _adapter->setSfxEnabled(enabled);
}

float GameState::getMusicVolume() const {
    return _adapter->getMusicVolume();
}

void GameState::setMusicVolume(float volume) {
    _adapter->setMusicVolume(volume);
}

std::shared_ptr<IGameStateManager> GameState::getGameStateManager() const {
    return _gameStateManager;
}

std::shared_ptr<IMapManager> GameState::getMapManager() const {
    return _gameStateManager->getMapManager();
}

std::shared_ptr<IPlayerManager> GameState::getPlayerManager() const {
    return _gameStateManager->getPlayerManager();
}

std::shared_ptr<IGameStatusManager> GameState::getGameStatusManager() const {
    return _gameStateManager->getGameStatusManager();
}

std::shared_ptr<IGameSettingsManager> GameState::getGameSettingsManager() const {
    return _gameStateManager->getGameSettingsManager();
}
