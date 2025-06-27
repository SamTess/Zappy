/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerManager
*/

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <memory>
#include "PlayerManager.hpp"
#include "../mapManager/IMapManager.hpp"

PlayerManager::PlayerManager() {
    _entityFactory = std::make_shared<EntityFactoryManager>();
}

PlayerManager::PlayerManager(std::shared_ptr<EntityFactoryManager> factory)
    : _entityFactory(std::move(factory)) {
}

void PlayerManager::setMapManager(std::shared_ptr<IMapManager> mapManager) {
    _mapManager = mapManager;
}

std::shared_ptr<const IPlayer> PlayerManager::getPlayerInfo(int playerId) const {
    auto it = _players.find(playerId);

    if (it != _players.end())
        return it->second;
    return nullptr;
}

std::vector<int> PlayerManager::getPlayerIds() const {
    std::vector<int> playerIds;
    playerIds.reserve(_players.size());
    for (const auto& pair : _players)
        playerIds.push_back(pair.first);
    return playerIds;
}

const std::map<int, std::shared_ptr<IPlayer>> PlayerManager::getPlayers() const {
    return _players;
}

void PlayerManager::addOrUpdatePlayer(const PlayerInfoData& playerData) {
    int playerId = playerData.getId();

    if (_players.find(playerId) != _players.end()) {
        const auto& oldPlayer = _players[playerId];
        int oldX = oldPlayer->getX();
        int oldY = oldPlayer->getY();
        _players[playerId]->updateFromProtocol(playerData);
        if ((oldX != playerData.getX() || oldY != playerData.getY()) && _mapManager) {
            _mapManager->removePlayerFromTile(playerId, oldX, oldY);
            _mapManager->addPlayerToTile(playerId, playerData.getX(), playerData.getY());
        }
    } else {
        if (_entityFactory)
            _players[playerId] = _entityFactory->getFactory().createPlayer(playerData);
        else
            _players[playerId] = std::make_shared<Player>(playerData);
        if (_mapManager)
            _mapManager->addPlayerToTile(playerId, playerData.getX(), playerData.getY());
    }
}

void PlayerManager::removePlayer(int playerId) {
    auto it = _players.find(playerId);
    if (it != _players.end()) {
        const auto& player = it->second;
        if (_mapManager)
            _mapManager->removePlayerFromTile(playerId, player->getX(), player->getY());
        _players.erase(it);
        _inventories.erase(playerId);
    }
}

void PlayerManager::movePlayer(int playerId, int newX, int newY) {
    auto it = _players.find(playerId);
    if (it != _players.end()) {
        auto& player = it->second;
        if (_mapManager) {
            _mapManager->removePlayerFromTile(playerId, player->getX(), player->getY());
            player->setPosition(newX, newY);
            _mapManager->addPlayerToTile(playerId, newX, newY);
        } else {
            player->setPosition(newX, newY);
        }
    }
}

std::shared_ptr<const IPlayerInventory> PlayerManager::getPlayerInventory(int playerId) const {
    auto it = _inventories.find(playerId);
    if (it != _inventories.end())
        return it->second;
    return nullptr;
}

void PlayerManager::updatePlayerInventory(const PlayerInventoryData& inventoryData) {
    int playerId = inventoryData.getId();
    auto it = _inventories.find(playerId);
    if (it != _inventories.end())
        it->second->updateFromProtocol(inventoryData);
    else
        _inventories[playerId] = _entityFactory->getFactory().createPlayerInventory(inventoryData);
}

std::shared_ptr<const IEgg> PlayerManager::getEggInfo(int eggId) const {
    auto it = _eggs.find(eggId);
    if (it != _eggs.end())
        return it->second;
    return nullptr;
}

void PlayerManager::addEgg(const EggData& eggData) {
    int eggId = eggData.getEggId();
    _eggs[eggId] = _entityFactory->getFactory().createEgg(eggData);
    if (_mapManager)
        _mapManager->addEggToTile(eggId, eggData.getX(), eggData.getY());
}

void PlayerManager::removeEgg(int eggId) {
    auto it = _eggs.find(eggId);
    if (it != _eggs.end()) {
        const auto& egg = it->second;
        if (_mapManager)
            _mapManager->removeEggFromTile(eggId, egg->getX(), egg->getY());
        _eggs.erase(it);
    }
}
