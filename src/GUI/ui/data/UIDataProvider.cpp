/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** UIDataProvider
*/

#include <algorithm>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "UIDataProvider.hpp"

namespace GUI {

UIDataProvider::UIDataProvider(std::shared_ptr<const GameState> gameState)
    : _gameState(gameState) {
}

void UIDataProvider::updateGameState(std::shared_ptr<const GameState> gameState) {
    _gameState = gameState;
}

void UIDataProvider::updateTimeData(float gameTime, int frequency, int gameTick) {
    _cachedGameTime = gameTime;
    _cachedFrequency = frequency;
    _cachedGameTick = gameTick;
}

bool UIDataProvider::isValidGameState() const {
    return _gameState != nullptr;
}

int UIDataProvider::getMapWidth() const {
    if (!isValidGameState()) return 0;
    return _gameState->getMapWidth();
}

int UIDataProvider::getMapHeight() const {
    if (!isValidGameState()) return 0;
    return _gameState->getMapHeight();
}

float UIDataProvider::getGameTime() const {
    return _cachedGameTime;
}

int UIDataProvider::getFrequency() const {
    return _cachedFrequency;
}

int UIDataProvider::getGameTick() const {
    return _cachedGameTick;
}

bool UIDataProvider::isMapInitialized() const {
    if (!isValidGameState()) return false;
    return _gameState->isMapInitialized();
}

bool UIDataProvider::isGameEnded() const {
    if (!isValidGameState()) return false;
    return _gameState->isGameEnded();
}

const std::string& UIDataProvider::getWinningTeam() const {
    if (!isValidGameState()) {
        static const std::string empty = "";
        return empty;
    }
    return _gameState->getWinningTeam();
}

std::shared_ptr<const ITile> UIDataProvider::getTile(int x, int y) const {
    if (!isValidGameState()) return nullptr;
    return _gameState->getTile(x, y);
}

int UIDataProvider::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    if (!isValidGameState()) return 0;
    return _gameState->getResourceQuantity(x, y, resourceType);
}

std::vector<int> UIDataProvider::getPlayersOnTile(int x, int y) const {
    if (!isValidGameState()) return {};
    return _gameState->getPlayersOnTile(x, y);
}

std::vector<int> UIDataProvider::getEggsOnTile(int x, int y) const {
    if (!isValidGameState()) return {};
    return _gameState->getEggsOnTile(x, y);
}

std::shared_ptr<const IPlayer> UIDataProvider::getPlayerInfo(int playerId) const {
    if (!isValidGameState()) return nullptr;
    return _gameState->getPlayerInfo(playerId);
}

std::shared_ptr<const IPlayerInventory> UIDataProvider::getPlayerInventory(int playerId) const {
    if (!isValidGameState()) return nullptr;
    return _gameState->getPlayerInventory(playerId);
}

bool UIDataProvider::isPlayerOnTile(int x, int y, int playerId) const {
    if (!isValidGameState()) return false;
    return _gameState->isPlayerOnTile(x, y, playerId);
}

std::vector<int> UIDataProvider::getPlayerIds() const {
    if (!isValidGameState()) return {};
    return _gameState->getPlayerIds();
}

const std::vector<std::string>& UIDataProvider::getTeamNames() const {
    if (!isValidGameState()) {
        static const std::vector<std::string> empty = {};
        return empty;
    }
    return _gameState->getTeamNames();
}

std::vector<int> UIDataProvider::calculateTotalResources() const {
    if (!isValidGameState() || !isMapInitialized())
        return std::vector<int>(static_cast<int>(ResourceType::COUNT), 0);
    std::vector<int> totals(static_cast<int>(ResourceType::COUNT), 0);
    for (int y = 0; y < getMapHeight(); ++y) {
        for (int x = 0; x < getMapWidth(); ++x) {
            for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i)
                totals[i] += getResourceQuantity(x, y, static_cast<ResourceType>(i));
        }
    }
    return totals;
}

std::map<std::string, int> UIDataProvider::getTeamPlayerCounts() const {
    std::map<std::string, int> teamCounts;
    if (!isValidGameState())
        return teamCounts;
    const auto& teamNames = getTeamNames();
    for (const auto& teamName : teamNames)
        teamCounts[teamName] = 0;
    const auto& playerIds = getPlayerIds();
    for (int playerId : playerIds) {
        auto player = getPlayerInfo(playerId);
        if (player) {
            const std::string& teamName = player->getTeamName();
            if (teamCounts.find(teamName) != teamCounts.end())
                teamCounts[teamName]++;
        }
    }
    return teamCounts;
}

std::map<std::string, std::vector<int>> UIDataProvider::getTeamResourceTotals() const {
    std::map<std::string, std::vector<int>> teamResources;
    if (!isValidGameState())
        return teamResources;
    const auto& teamNames = getTeamNames();
    for (const auto& teamName : teamNames)
        teamResources[teamName] = std::vector<int>(static_cast<int>(ResourceType::COUNT), 0);
    const auto& playerIds = getPlayerIds();
    for (int playerId : playerIds) {
        auto player = getPlayerInfo(playerId);
        auto inventory = getPlayerInventory(playerId);
        if (player && inventory) {
            const std::string& teamName = player->getTeamName();
            if (teamResources.find(teamName) != teamResources.end()) {
                auto& resources = teamResources[teamName];
                resources[static_cast<int>(ResourceType::FOOD)] += inventory->getFood();
                resources[static_cast<int>(ResourceType::LINEMATE)] += inventory->getLinemate();
                resources[static_cast<int>(ResourceType::DERAUMERE)] += inventory->getDeraumere();
                resources[static_cast<int>(ResourceType::SIBUR)] += inventory->getSibur();
                resources[static_cast<int>(ResourceType::MENDIANE)] += inventory->getMendiane();
                resources[static_cast<int>(ResourceType::PHIRAS)] += inventory->getPhiras();
                resources[static_cast<int>(ResourceType::THYSTAME)] += inventory->getThystame();
            }
        }
    }
    return teamResources;
}

} // namespace GUI
