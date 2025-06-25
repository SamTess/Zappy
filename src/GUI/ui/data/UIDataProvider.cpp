/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UIDataProvider implementation
*/

#include <algorithm>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "UIDataProvider.hpp"

namespace GUI {

UIDataProvider::UIDataProvider(std::shared_ptr<const GameState> gameState)
    : m_gameState(gameState) {
}

void UIDataProvider::updateGameState(std::shared_ptr<const GameState> gameState) {
    m_gameState = gameState;
}

void UIDataProvider::updateTimeData(float gameTime, int frequency, int gameTick) {
    m_cachedGameTime = gameTime;
    m_cachedFrequency = frequency;
    m_cachedGameTick = gameTick;
}

bool UIDataProvider::isValidGameState() const {
    return m_gameState != nullptr;
}

int UIDataProvider::getMapWidth() const {
    if (!isValidGameState()) return 0;
    return m_gameState->getMapWidth();
}

int UIDataProvider::getMapHeight() const {
    if (!isValidGameState()) return 0;
    return m_gameState->getMapHeight();
}

float UIDataProvider::getGameTime() const {
    return m_cachedGameTime;
}

int UIDataProvider::getFrequency() const {
    return m_cachedFrequency;
}

int UIDataProvider::getGameTick() const {
    return m_cachedGameTick;
}

bool UIDataProvider::isMapInitialized() const {
    if (!isValidGameState()) return false;
    return m_gameState->isMapInitialized();
}

bool UIDataProvider::isGameEnded() const {
    if (!isValidGameState()) return false;
    return m_gameState->isGameEnded();
}

const std::string& UIDataProvider::getWinningTeam() const {
    if (!isValidGameState()) {
        static const std::string empty = "";
        return empty;
    }
    return m_gameState->getWinningTeam();
}

std::shared_ptr<const ITile> UIDataProvider::getTile(int x, int y) const {
    if (!isValidGameState()) return nullptr;
    return m_gameState->getTile(x, y);
}

int UIDataProvider::getResourceQuantity(int x, int y, ResourceType resourceType) const {
    if (!isValidGameState()) return 0;
    return m_gameState->getResourceQuantity(x, y, resourceType);
}

ResourceType UIDataProvider::getDominantResourceType(int x, int y) const {
    if (!isValidGameState()) return ResourceType::FOOD;
    return m_gameState->getDominantResourceType(x, y);
}

std::vector<int> UIDataProvider::getPlayersOnTile(int x, int y) const {
    if (!isValidGameState()) return {};
    return m_gameState->getPlayersOnTile(x, y);
}

std::vector<int> UIDataProvider::getEggsOnTile(int x, int y) const {
    if (!isValidGameState()) return {};
    return m_gameState->getEggsOnTile(x, y);
}

std::shared_ptr<const IPlayer> UIDataProvider::getPlayerInfo(int playerId) const {
    if (!isValidGameState()) return nullptr;
    return m_gameState->getPlayerInfo(playerId);
}

std::shared_ptr<const IPlayerInventory> UIDataProvider::getPlayerInventory(int playerId) const {
    if (!isValidGameState()) return nullptr;
    return m_gameState->getPlayerInventory(playerId);
}

bool UIDataProvider::isPlayerOnTile(int x, int y, int playerId) const {
    if (!isValidGameState()) return false;
    return m_gameState->isPlayerOnTile(x, y, playerId);
}

std::vector<int> UIDataProvider::getPlayerIds() const {
    if (!isValidGameState()) return {};
    return m_gameState->getPlayerIds();
}

const std::vector<std::string>& UIDataProvider::getTeamNames() const {
    if (!isValidGameState()) {
        static const std::vector<std::string> empty = {};
        return empty;
    }
    return m_gameState->getTeamNames();
}

std::vector<std::shared_ptr<const IBroadcast>> UIDataProvider::getBroadcasts() const {
    if (!isValidGameState()) {
        return {};
    }
    return m_gameState->getBroadcasts();
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
