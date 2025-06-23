/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameLoop UI Implementation
*/

#include <iostream>
#include <memory>
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
/**
 * Met à jour les données de jeu et le GameState
 */
void GameLoop::updateGameData() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    m_gameTime += deltaTime;
    m_gameTick++;
    if (m_gameController) {
        m_gameController->updateBroadcasts(deltaTime);
        auto gameState = m_gameController->getGameState();
        if (gameState && gameState->isMapInitialized()) {
            m_mapWidth = gameState->getMapWidth();
            m_mapHeight = gameState->getMapHeight();
            m_frequency = gameState->getTimeUnit();
        }
    }
    if (!m_userInterface->hasHandledMouseEvent() &&
        !m_userInterface->isMouseOverUI() &&
        m_graphics->IsMouseButtonPressed(0)) {
        ZappyTypes::Vector2 mousePos = m_graphics->GetMousePosition();
        int tileX = static_cast<int>(mousePos.x / 32) % m_mapWidth;
        int tileY = static_cast<int>(mousePos.y / 32) % m_mapHeight;
        handleTileSelection(tileX, tileY);
    }
    if (!m_userInterface->hasHandledMouseEvent() &&
        !m_userInterface->isMouseOverUI() &&
        m_graphics->IsMouseButtonPressed(0)) {
        ZappyTypes::Vector2 mousePos = m_graphics->GetMousePosition();
        int tileX = static_cast<int>(mousePos.x / 32) % m_mapWidth;
        int tileY = static_cast<int>(mousePos.y / 32) % m_mapHeight;
        handleTileSelection(tileX, tileY);
    }
}

/**
 * Gère la sélection d'une case par l'utilisateur
 */
void GameLoop::handleTileSelection(int x, int y) {
    if (x < 0 || y < 0 || x >= m_mapWidth || y >= m_mapHeight)
        return;
    m_userInterface->setSelectedTile(x, y);
    if (m_gameController) {
        auto gameState = m_gameController->getGameState();
        auto tile = gameState->getTile(x, y);
        m_selectedTile.x = x;
        m_selectedTile.y = y;
        m_selectedTile.selected = true;
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
    updateCameraForMapSize();
}

void GameLoop::onTileChanged(int x, int y, const std::shared_ptr<const ITile>& tile) {
    if (m_gameController && tile) {
        auto gameState = m_gameController->getGameState();
        if (gameState && gameState->isMapInitialized()) {
            auto mutableGameState = std::const_pointer_cast<GameState>(gameState);
            const auto& resources = tile->getResources();
            mutableGameState->updateTileResources(x, y,
                resources[0],  // food
                resources[1],  // linemate
                resources[2],  // deraumere
                resources[3],  // sibur
                resources[4],  // mendiane
                resources[5],  // phiras
                resources[6]   // thystame
            );
        }
    }
}
