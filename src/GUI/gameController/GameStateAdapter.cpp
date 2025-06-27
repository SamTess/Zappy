/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameStateAdapter
*/

#include <utility>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "GameStateAdapter.hpp"

GameStateAdapter::GameStateAdapter(std::shared_ptr<IGameStateManager> gameStateManager)
    : _gameStateManager(std::move(gameStateManager)) {
}

int GameStateAdapter::getMapWidth() const {
    return _gameStateManager->getMapManager()->getMapWidth();
}

int GameStateAdapter::getMapHeight() const {
    return _gameStateManager->getMapManager()->getMapHeight();
}

bool GameStateAdapter::isMapInitialized() const {
    return _gameStateManager->getMapManager()->isMapInitialized();
}

std::shared_ptr<const ITile> GameStateAdapter::getTile(int x, int y) const {
    return _gameStateManager->getMapManager()->getTile(x, y);
}

std::shared_ptr<ITile> GameStateAdapter::getTileMutable(int x, int y) {
    return _gameStateManager->getMapManager()->getTileMutable(x, y);
}

int GameStateAdapter::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    return _gameStateManager->getMapManager()->getResourceQuantity(x, y, resourceType);
}

void GameStateAdapter::setMapSize(int width, int height) {
    _gameStateManager->getMapManager()->setMapSize(width, height);
}

void GameStateAdapter::updateTileResources(int x, int y, int food, int linemate, int deraumere,
    int sibur, int mendiane, int phiras, int thystame) {
    _gameStateManager->getMapManager()->updateTileResources(x, y, food, linemate, deraumere,
        sibur, mendiane, phiras, thystame);
}

void GameStateAdapter::setTileIncantationState(int x, int y, bool isIncantating) {
    _gameStateManager->getMapManager()->setTileIncantationState(x, y, isIncantating);
}

bool GameStateAdapter::isPlayerOnTile(int x, int y, int playerId) const {
    return _gameStateManager->getMapManager()->isPlayerOnTile(x, y, playerId);
}

std::vector<int> GameStateAdapter::getPlayersOnTile(int x, int y) const {
    return _gameStateManager->getMapManager()->getPlayersOnTile(x, y);
}

std::vector<int> GameStateAdapter::getEggsOnTile(int x, int y) const {
    return _gameStateManager->getMapManager()->getEggsOnTile(x, y);
}

std::shared_ptr<const IPlayer> GameStateAdapter::getPlayerInfo(int playerId) const {
    return _gameStateManager->getPlayerManager()->getPlayerInfo(playerId);
}

std::shared_ptr<const IPlayerInventory> GameStateAdapter::getPlayerInventory(int playerId) const {
    return _gameStateManager->getPlayerManager()->getPlayerInventory(playerId);
}

std::vector<int> GameStateAdapter::getPlayerIds() const {
    return _gameStateManager->getPlayerManager()->getPlayerIds();
}

const std::map<int, std::shared_ptr<IPlayer>> GameStateAdapter::getPlayers() const {
    return _gameStateManager->getPlayerManager()->getPlayers();
}

void GameStateAdapter::addOrUpdatePlayer(const PlayerInfoData& playerData) {
    _gameStateManager->getPlayerManager()->addOrUpdatePlayer(playerData);
}

void GameStateAdapter::removePlayer(int playerId) {
    _gameStateManager->getPlayerManager()->removePlayer(playerId);
}

void GameStateAdapter::updatePlayerInventory(const PlayerInventoryData& inventoryData) {
    _gameStateManager->getPlayerManager()->updatePlayerInventory(inventoryData);
}

void GameStateAdapter::movePlayer(int playerId, int newX, int newY) {
    _gameStateManager->getPlayerManager()->movePlayer(playerId, newX, newY);
}

std::shared_ptr<const IEgg> GameStateAdapter::getEggInfo(int eggId) const {
    return _gameStateManager->getPlayerManager()->getEggInfo(eggId);
}

void GameStateAdapter::addEgg(const EggData& eggData) {
    _gameStateManager->getPlayerManager()->addEgg(eggData);
}

void GameStateAdapter::removeEgg(int eggId) {
    _gameStateManager->getPlayerManager()->removeEgg(eggId);
}

const std::vector<std::string>& GameStateAdapter::getTeamNames() const {
    return _gameStateManager->getGameStatusManager()->getTeamNames();
}

void GameStateAdapter::setTeamNames(const std::vector<std::string>& teamNames) {
    _gameStateManager->getGameStatusManager()->setTeamNames(teamNames);
}

int GameStateAdapter::getTimeUnit() const {
    return _gameStateManager->getGameStatusManager()->getTimeUnit();
}

void GameStateAdapter::setTimeUnit(int timeUnit) {
    _gameStateManager->getGameStatusManager()->setTimeUnit(timeUnit);
}

bool GameStateAdapter::isGameEnded() const {
    return _gameStateManager->getGameStatusManager()->isGameEnded();
}

const std::string& GameStateAdapter::getWinningTeam() const {
    return _gameStateManager->getGameStatusManager()->getWinningTeam();
}

void GameStateAdapter::setGameEnded(bool ended, const std::string& winningTeam) {
    _gameStateManager->getGameStatusManager()->setGameEnded(ended, winningTeam);
}

bool GameStateAdapter::getSfxEnabled() const {
    return _gameStateManager->getGameSettingsManager()->getSfxEnabled();
}

void GameStateAdapter::setSfxEnabled(bool enabled) {
    _gameStateManager->getGameSettingsManager()->setSfxEnabled(enabled);
}

float GameStateAdapter::getMusicVolume() const {
    return _gameStateManager->getGameSettingsManager()->getMusicVolume();
}

void GameStateAdapter::setMusicVolume(float volume) {
    _gameStateManager->getGameSettingsManager()->setMusicVolume(volume);
}

std::shared_ptr<IGameStateManager> GameStateAdapter::getGameStateManager() const {
    return _gameStateManager;
}
