/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameStateManager
*/

#include <utility>
#include "GameStateManager.hpp"

GameStateManager::GameStateManager()
    : _isInitialized(false) {
    _entityFactory = std::make_shared<EntityFactoryManager>();
    initialize();
}

GameStateManager::GameStateManager(std::shared_ptr<EntityFactoryManager> factory)
    : _entityFactory(std::move(factory)), _isInitialized(false) {
    initialize();
}

void GameStateManager::initialize() {
    if (_isInitialized)
        return;
    _mapManager = std::make_shared<MapManager>(_entityFactory);
    _playerManager = std::make_shared<PlayerManager>(_entityFactory);
    _gameStatusManager = std::make_shared<GameStatusManager>();
    _gameSettingsManager = std::make_shared<GameSettingsManager>();
    _playerManager->setMapManager(_mapManager);
    _isInitialized = true;
}

bool GameStateManager::isInitialized() const {
    return _isInitialized;
}

std::shared_ptr<IMapManager> GameStateManager::getMapManager() {
    return _mapManager;
}

std::shared_ptr<const IMapManager> GameStateManager::getMapManager() const {
    return _mapManager;
}

std::shared_ptr<IPlayerManager> GameStateManager::getPlayerManager() {
    return _playerManager;
}

std::shared_ptr<const IPlayerManager> GameStateManager::getPlayerManager() const {
    return _playerManager;
}

std::shared_ptr<IGameStatusManager> GameStateManager::getGameStatusManager() {
    return _gameStatusManager;
}

std::shared_ptr<const IGameStatusManager> GameStateManager::getGameStatusManager() const {
    return _gameStatusManager;
}

std::shared_ptr<IGameSettingsManager> GameStateManager::getGameSettingsManager() {
    return _gameSettingsManager;
}

std::shared_ptr<const IGameSettingsManager> GameStateManager::getGameSettingsManager() const {
    return _gameSettingsManager;
}
