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
#include <limits>
#include <cmath>
#include <map>
#include "GameLoop.hpp"
#include "gameController/GameState.hpp"

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
        m_gameController->updateAnimations(deltaTime);
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
        auto playerId = std::make_shared<int>(0);
        if (performPlayerSelection(mousePos, playerId)) {
            handlePlayerSelection(*playerId);
        } else {
            auto tileX = std::make_shared<int>(0);
            auto tileY = std::make_shared<int>(0);
            if (performTileSelection(mousePos, tileX, tileY)) {
                handleTileSelection(*tileX, *tileY);
            }
        }
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
        m_selectedPlayer.selected = false;
    }
}

/**
 * Gère la sélection d'un joueur par l'utilisateur
 */
void GameLoop::handlePlayerSelection(int playerId) {
    if (playerId < 0)
        return;
    m_userInterface->setSelectedPlayer(playerId);
    m_selectedPlayer.playerId = playerId;
    m_selectedPlayer.selected = true;
    m_selectedTile.selected = false;
}

/**
 * Convertit les coordonnées du monde 3D en coordonnées de tuiles
 */
bool GameLoop::worldToTileCoordinates(ZappyTypes::Vector3 worldPos, std::shared_ptr<int> tileX, std::shared_ptr<int> tileY) {
    if (m_mapWidth <= 0 || m_mapHeight <= 0)
        return false;
    float tileSize = 1.0f;
    float spacing = 1.5f;
    if (m_mapWidth > 20 || m_mapHeight > 20) {
        tileSize = 10.0f / std::max(m_mapWidth, m_mapHeight);
        spacing = tileSize * 0.1f;
    }
    float mapCenterX = m_mapWidth / 2.0f;
    float mapCenterY = m_mapHeight / 2.0f;
    float x = (worldPos.x / (tileSize + spacing)) + mapCenterX - 0.5f;
    float y = (worldPos.z / (tileSize + spacing)) + mapCenterY - 0.5f;
    *tileX = static_cast<int>(std::round(x));
    *tileY = static_cast<int>(std::round(y));
    return (*tileX >= 0 && *tileX < m_mapWidth && *tileY >= 0 && *tileY < m_mapHeight);
}

/**
 * Effectue un ray casting pour la sélection de tuiles 3D
 */
bool GameLoop::performTileSelection(ZappyTypes::Vector2 screenPos, std::shared_ptr<int> tileX, std::shared_ptr<int> tileY) {
    if (!m_graphics)
        return false;
    ZappyTypes::Vector3 cameraPos = m_graphics->GetCameraPosition();
    ZappyTypes::Vector3 rayDirection = m_graphics->ScreenToWorldRay(screenPos);
    ZappyTypes::Vector3 planePoint = {0.0f, 0.0f, 0.0f};
    ZappyTypes::Vector3 planeNormal = {0.0f, 1.0f, 0.0f};
    ZappyTypes::Vector3 intersectionPoint;
    if (m_graphics->RayPlaneIntersection(cameraPos, rayDirection,
        planePoint, planeNormal, intersectionPoint))
        return worldToTileCoordinates(intersectionPoint, tileX, tileY);
    return false;
}

/**
 * Calcule la distance entre un rayon 3D et un point
 */
float GameLoop::calculateRayToPointDistance(ZappyTypes::Vector3 rayOrigin, ZappyTypes::Vector3 rayDirection, ZappyTypes::Vector3 point) {
    ZappyTypes::Vector3 rayToPoint = {
        point.x - rayOrigin.x,
        point.y - rayOrigin.y,
        point.z - rayOrigin.z
    };
    float dotProduct = rayToPoint.x * rayDirection.x + rayToPoint.y * rayDirection.y + rayToPoint.z * rayDirection.z;
    ZappyTypes::Vector3 closestPointOnRay = {
        rayOrigin.x + dotProduct * rayDirection.x,
        rayOrigin.y + dotProduct * rayDirection.y,
        rayOrigin.z + dotProduct * rayDirection.z
    };
    ZappyTypes::Vector3 diff = {
        point.x - closestPointOnRay.x,
        point.y - closestPointOnRay.y,
        point.z - closestPointOnRay.z
    };
    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

/**
 * Calcule la position 3D d'un joueur basée sur sa position sur la tuile
 */
ZappyTypes::Vector3 GameLoop::calculatePlayerWorldPosition(int playerX, int playerY, int playerIndex, int totalPlayers) {
    float tileSize = 1.0f;
    float spacing = 1.5f;
    if (m_mapWidth > 20 || m_mapHeight > 20) {
        tileSize = 10.0f / std::max(m_mapWidth, m_mapHeight);
        spacing = tileSize * 0.1f;
    }
    float mapCenterX = m_mapWidth / 2.0f;
    float mapCenterY = m_mapHeight / 2.0f;
    ZappyTypes::Vector3 playerPos = {
        (playerX - mapCenterX + 0.5f) * (tileSize + spacing),
        0.55f,
        (playerY - mapCenterY + 0.5f) * (tileSize + spacing)
    };
    if (totalPlayers > 1) {
        float stackHeight = 1.1f;
        playerPos.y += playerIndex * stackHeight;
    }
    return playerPos;
}

/**
 * Effectue la sélection d'un joueur en 3D avec ray casting précis
 */
bool GameLoop::performPlayerSelection(ZappyTypes::Vector2 screenPos, std::shared_ptr<int> playerId) {
    if (!m_gameController || !m_graphics)
        return false;
    auto gameState = m_gameController->getGameState();
    if (!gameState || !gameState->isMapInitialized())
        return false;
    ZappyTypes::Vector3 cameraPos = m_graphics->GetCameraPosition();
    ZappyTypes::Vector3 rayDirection = m_graphics->ScreenToWorldRay(screenPos);
    float minDistance = std::numeric_limits<float>::max();
    int closestPlayerId = -1;
    bool foundPlayer = false;
    for (int y = 0; y < m_mapHeight; ++y) {
        for (int x = 0; x < m_mapWidth; ++x) {
            auto playerIds = gameState->getPlayersOnTile(x, y);
            if (playerIds.empty())
                continue;
            for (size_t i = 0; i < playerIds.size(); ++i) {
                int currentPlayerId = playerIds[i];
                ZappyTypes::Vector3 playerWorldPos = calculatePlayerWorldPosition(x, y, i, playerIds.size());
                float distance = calculateRayToPointDistance(cameraPos, rayDirection, playerWorldPos);
                const float maxSelectionDistance = 1.0f;
                if (distance < maxSelectionDistance && distance < minDistance) {
                    minDistance = distance;
                    closestPlayerId = currentPlayerId;
                    foundPlayer = true;
                }
            }
        }
    }
    if (foundPlayer) {
        *playerId = closestPlayerId;
        return true;
    }
    return false;
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
