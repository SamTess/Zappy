/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameLoop UI Implementation
*/

#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include "GameLoop.hpp"
#include "gameController/GameState.hpp"

// Timers pour les incantations
static std::map<int, float> playerIncantationTimers;

/**
 * Met à jour les données de jeu et rafraîchit l'interface utilisateur
 */
void GameLoop::updateGameData() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    m_gameData.time += deltaTime;
    m_gameData.tick++;
    for (auto it = playerIncantationTimers.begin(); it != playerIncantationTimers.end(); ) {
        int playerId = it->first;
        float& timer = it->second;
        timer -= deltaTime;
        if (timer <= 0.0f) {
            for (auto& player : m_gameData.players) {
                if (player.id == playerId) {
                    player.isIncantating = false;
                    break;
                }
            }
            it = playerIncantationTimers.erase(it);
        } else {
            ++it;
        }
    }
    if (m_gameController) {
        m_gameController->updateBroadcasts(deltaTime);
        auto gameState = m_gameController->getGameState();
        if (gameState->isMapInitialized()) {
            m_gameData.mapWidth = gameState->getMapWidth();
            m_gameData.mapHeight = gameState->getMapHeight();
            m_gameData.frequency = gameState->getTimeUnit();
            updateTilesFromGameState(gameState);
            updatePlayersFromGameState(gameState);
            updateTeamsFromGameState(gameState);
            updateBroadcastsFromGameState(gameState);
        }
    }
    bool uiHandledMouseEvent = m_userInterface->hasHandledMouseEvent();
    bool mouseOverUI = m_userInterface->isMouseOverUI();
    if (!uiHandledMouseEvent && !mouseOverUI && m_graphics->IsMouseButtonPressed(0)) {
        ZappyTypes::Vector2 mousePos = m_graphics->GetMousePosition();
        int tileX = static_cast<int>(mousePos.x / 32) % m_gameData.mapWidth;
        int tileY = static_cast<int>(mousePos.y / 32) % m_gameData.mapHeight;
        handleTileSelection(tileX, tileY);
    }
    m_userInterface->updateData(m_gameData);
}

/**
 * Gère la sélection d'une case par l'utilisateur
 */
void GameLoop::handleTileSelection(int x, int y) {
    if (x < 0 || y < 0 || x >= m_gameData.mapWidth || y >= m_gameData.mapHeight)
        return;

    m_userInterface->setSelectedTile(x, y);
    auto playerIt = std::find_if(m_gameData.players.begin(), m_gameData.players.end(),
        [x, y](const auto& player) {
            return player.x == x && player.y == y;
        });

    if (playerIt != m_gameData.players.end()) {
        std::stringstream message;
        message << "Joueur " << playerIt->id << " [Équipe: " << playerIt->team << ", Niveau: " << playerIt->level << "] sélectionné";
        m_userInterface->addLogMessage(message.str());
    }
    auto tileIt = std::find_if(m_gameData.tiles.begin(), m_gameData.tiles.end(),
        [x, y](const auto& tile) {
            return tile.x == x && tile.y == y;
        });

    if (tileIt == m_gameData.tiles.end()) {
        GUI::Tile newTile;
        newTile.x = x;
        newTile.y = y;
        m_gameData.tiles.push_back(newTile);
    }
}

/**
 * Change le mode de visualisation
 */
void GameLoop::handleViewModeChange(int mode) {
    m_userInterface->setViewMode(mode);
    switch (mode) {
        case 0: // Standard
            // Configuration caméra standard
            break;
        case 1: // Aérien
            // Configuration caméra aérienne (top-down)
            break;
        case 2: // Wireframe
            // Mode wireframe
            break;
        case 3: // Ressources
            // Mode affichant les ressources
            break;
    }
}

void GameLoop::onMapSizeChanged(int width, int height) {
    m_mapWidth = width;
    m_mapHeight = height;
    m_camera->setMapDimensions(width, height);
    m_gameData.mapWidth = width;
    m_gameData.mapHeight = height;
    std::stringstream message;
    message << "Dimensions de la map modifiées : " << width << "x" << height;
    m_userInterface->addLogMessage(message.str());
}

void GameLoop::onTileChanged(int x, int y, const TileData& tileData) {
    auto tileIt = std::find_if(m_gameData.tiles.begin(), m_gameData.tiles.end(),
        [x, y](const auto& tile) {
            return tile.x == x && tile.y == y;
        });
    GUI::Tile* targetTile;
    if (tileIt == m_gameData.tiles.end()) {
        GUI::Tile newTile;
        newTile.x = x;
        newTile.y = y;
        m_gameData.tiles.push_back(newTile);
        targetTile = &m_gameData.tiles.back();
    } else {
        targetTile = &(*tileIt);
    }
    targetTile->food = tileData.resources[0]; // ResourceType::FOOD
    targetTile->linemate = tileData.resources[1]; // ResourceType::LINEMATE
    targetTile->deraumere = tileData.resources[2]; // ResourceType::DERAUMERE
    targetTile->sibur = tileData.resources[3]; // ResourceType::SIBUR
    targetTile->mendiane = tileData.resources[4]; // ResourceType::MENDIANE
    targetTile->phiras = tileData.resources[5]; // ResourceType::PHIRAS
    targetTile->thystame = tileData.resources[6]; // ResourceType::THYSTAME
    if (m_selectedTile.selected && m_selectedTile.x == x && m_selectedTile.y == y) {
        // Rafraîchir les informations affichées
    }
}

/**
 * Met à jour les tuiles depuis le GameState
 */
void GameLoop::updateTilesFromGameState(std::shared_ptr<const GameState> gameState) {
    int mapWidth = gameState->getMapWidth();
    int mapHeight = gameState->getMapHeight();
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            const TileData& tileData = gameState->getTileData(x, y);
            auto tileIt = std::find_if(m_gameData.tiles.begin(), m_gameData.tiles.end(),
                [x, y](const GUI::Tile& tile) {
                    return tile.x == x && tile.y == y;
                });
            GUI::Tile* targetTile;
            if (tileIt == m_gameData.tiles.end()) {
                GUI::Tile newTile;
                newTile.x = x;
                newTile.y = y;
                m_gameData.tiles.push_back(newTile);
                targetTile = &m_gameData.tiles.back();
            } else {
                targetTile = &(*tileIt);
            }
            targetTile->food = tileData.resources[static_cast<int>(ResourceType::FOOD)];
            targetTile->linemate = tileData.resources[static_cast<int>(ResourceType::LINEMATE)];
            targetTile->deraumere = tileData.resources[static_cast<int>(ResourceType::DERAUMERE)];
            targetTile->sibur = tileData.resources[static_cast<int>(ResourceType::SIBUR)];
            targetTile->mendiane = tileData.resources[static_cast<int>(ResourceType::MENDIANE)];
            targetTile->phiras = tileData.resources[static_cast<int>(ResourceType::PHIRAS)];
            targetTile->thystame = tileData.resources[static_cast<int>(ResourceType::THYSTAME)];
        }
    }
}

/**
 * Met à jour les joueurs depuis le GameState
 */
void GameLoop::updatePlayersFromGameState(std::shared_ptr<const GameState> gameState) {
    int mapWidth = gameState->getMapWidth();
    int mapHeight = gameState->getMapHeight();
    std::vector<int> updatedPlayerIds;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            const TileData& tileData = gameState->getTileData(x, y);
            for (int playerId : tileData.playerIds) {
                updatedPlayerIds.push_back(playerId);
                auto playerInfo = gameState->getPlayerInfo(playerId);
                auto playerInventory = gameState->getPlayerInventory(playerId);
                if (playerInfo) {
                    auto playerIt = std::find_if(m_gameData.players.begin(), m_gameData.players.end(),
                        [playerId](const GUI::Player& player) {
                            return player.id == playerId;
                        });
                    GUI::Player* targetPlayer;
                    if (playerIt == m_gameData.players.end()) {
                        GUI::Player newPlayer;
                        newPlayer.id = playerId;
                        m_gameData.players.push_back(newPlayer);
                        targetPlayer = &m_gameData.players.back();
                    } else {
                        targetPlayer = &(*playerIt);
                    }
                    targetPlayer->team = playerInfo->getTeamName();
                    targetPlayer->x = playerInfo->getX();
                    targetPlayer->y = playerInfo->getY();
                    targetPlayer->orientation = playerInfo->getOrientation();
                    targetPlayer->level = playerInfo->getLevel();
                    if (playerInventory) {
                        targetPlayer->inventory.food = playerInventory->getFood();
                        targetPlayer->inventory.linemate = playerInventory->getLinemate();
                        targetPlayer->inventory.deraumere = playerInventory->getDeraumere();
                        targetPlayer->inventory.sibur = playerInventory->getSibur();
                        targetPlayer->inventory.mendiane = playerInventory->getMendiane();
                        targetPlayer->inventory.phiras = playerInventory->getPhiras();
                        targetPlayer->inventory.thystame = playerInventory->getThystame();
                    }
                }
            }
        }
    }
    m_gameData.players.erase(
        std::remove_if(m_gameData.players.begin(), m_gameData.players.end(),
            [&updatedPlayerIds](const GUI::Player& player) {
                return std::find(updatedPlayerIds.begin(), updatedPlayerIds.end(), player.id) == updatedPlayerIds.end();
            }),
        m_gameData.players.end()
    );
}

/**
 * Met à jour les équipes depuis le GameState
 */
void GameLoop::updateTeamsFromGameState(std::shared_ptr<const GameState> gameState) {
    const std::vector<std::string>& teamNames = gameState->getTeamNames();
    for (const auto& teamName : teamNames) {
        auto teamIt = std::find_if(m_gameData.teams.begin(), m_gameData.teams.end(),
            [&teamName](const GUI::Team& team) {
                return team.name == teamName;
            });
        if (teamIt == m_gameData.teams.end()) {
            GUI::Team newTeam;
            newTeam.name = teamName;
            m_gameData.teams.push_back(newTeam);
        }
    }
    m_gameData.teams.erase(
        std::remove_if(m_gameData.teams.begin(), m_gameData.teams.end(),
            [&teamNames](const GUI::Team& team) {
                return std::find(teamNames.begin(), teamNames.end(), team.name) == teamNames.end();
            }),
        m_gameData.teams.end()
    );
}

/**
 * Met à jour les broadcasts depuis le GameState
 */
void GameLoop::updateBroadcastsFromGameState(std::shared_ptr<const GameState> gameState) {
    const auto& stateBroadcasts = gameState->getBroadcasts();
    m_gameData.broadcasts.clear();
    for (const auto& stateBroadcast : stateBroadcasts) {
        GUI::Broadcast guiBroadcast;
        guiBroadcast.team = stateBroadcast.team;
        guiBroadcast.message = stateBroadcast.message;
        guiBroadcast.timeLeft = stateBroadcast.timeLeft;
        guiBroadcast.playerId = stateBroadcast.playerId;
        m_gameData.broadcasts.push_back(guiBroadcast);
    }
}
