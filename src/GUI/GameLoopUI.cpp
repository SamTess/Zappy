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
#include <map>
#include "GameLoop.hpp"

// Timers pour les incantations des joueurs
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
    if (m_graphics->IsMouseButtonPressed(0)) {
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

    // Utilisation de l'interface utilisateur qui délègue maintenant à UIWindowFactory
    m_userInterface->setSelectedTile(x, y);

    // Rechercher si un joueur est sur cette case
    auto playerIt = std::find_if(m_gameData.players.begin(), m_gameData.players.end(),
        [x, y](const auto& player) {
            return player.x == x && player.y == y;
        });

    if (playerIt != m_gameData.players.end()) {
        std::stringstream message;
        message << "Joueur " << playerIt->id << " [Équipe: " << playerIt->team << ", Niveau: " << playerIt->level << "] sélectionné";
        m_userInterface->addLogMessage(message.str());
    }

    // Rechercher les ressources sur cette case
    auto tileIt = std::find_if(m_gameData.tiles.begin(), m_gameData.tiles.end(),
        [x, y](const auto& tile) {
            return tile.x == x && tile.y == y;
        });

    if (tileIt == m_gameData.tiles.end()) {
        // Créer une nouvelle case s'il n'y en a pas
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
    // Utilise UIWindowFactory pour changer le mode d'affichage
    m_userInterface->setViewMode(mode);
    // Adapter la caméra ou le rendu selon le mode
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
