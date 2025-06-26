/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameLoopUI
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

void GameLoop::updateGameData() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    _gameTime += deltaTime;
    _gameTick++;
    if (_gameController) {
        _gameController->updateBroadcasts(deltaTime);
        _gameController->updateAnimations(deltaTime);
        auto gameState = _gameController->getGameState();
        if (gameState && gameState->isMapInitialized()) {
            _mapWidth = gameState->getMapWidth();
            _mapHeight = gameState->getMapHeight();
            _frequency = gameState->getTimeUnit();
        }
        if (_userInterface)
            _userInterface->updateDataFromGameState(gameState);
    }
    if (_userInterface && _graphics &&
        !_userInterface->hasHandledMouseEvent() &&
        !_userInterface->isMouseOverUI() &&
        _graphics->IsMouseButtonPressed(0)) {
        ZappyTypes::Vector2 mousePos = _graphics->GetMousePosition();
        auto playerId = std::make_shared<int>(0);
        if (performPlayerSelection(mousePos, playerId)) {
            handlePlayerSelection(*playerId);
        } else {
            auto tileX = std::make_shared<int>(0);
            auto tileY = std::make_shared<int>(0);
            if (performTileSelection(mousePos, tileX, tileY))
                handleTileSelection(*tileX, *tileY);
        }
    }
}

void GameLoop::handleTileSelection(int x, int y) {
    if (x < 0 || y < 0 || x >= _mapWidth || y >= _mapHeight)
        return;
    _userInterface->setSelectedTile(x, y);
    if (_gameController) {
        auto gameState = _gameController->getGameState();
        auto tile = gameState->getTile(x, y);
        _selectedTile.x = x;
        _selectedTile.y = y;
        _selectedTile.selected = true;
        _selectedPlayer.selected = false;
    }
}

void GameLoop::handlePlayerSelection(int playerId) {
    if (playerId < 0)
        return;
    _userInterface->setSelectedPlayer(playerId);
    _selectedPlayer.playerId = playerId;
    _selectedPlayer.selected = true;
    _selectedTile.selected = false;
}


bool GameLoop::worldToTileCoordinates(ZappyTypes::Vector3 worldPos, std::shared_ptr<int> tileX, std::shared_ptr<int> tileY) {
    if (_mapWidth <= 0 || _mapHeight <= 0)
        return false;
    float tileSize = 1.0f;
    float spacing = 1.5f;
    if (_mapWidth > 20 || _mapHeight > 20) {
        tileSize = 10.0f / std::max(_mapWidth, _mapHeight);
        spacing = tileSize * 0.1f;
    }
    float mapCenterX = _mapWidth / 2.0f;
    float mapCenterY = _mapHeight / 2.0f;
    float x = (worldPos.x / (tileSize + spacing)) + mapCenterX - 0.5f;
    float y = (worldPos.z / (tileSize + spacing)) + mapCenterY - 0.5f;
    *tileX = static_cast<int>(std::round(x));
    *tileY = static_cast<int>(std::round(y));
    if (*tileX >= 0 && *tileX < _mapWidth && *tileY >= 0 && *tileY < _mapHeight)
        return true;
    return false;
}

bool GameLoop::performTileSelection(ZappyTypes::Vector2 screenPos, std::shared_ptr<int> tileX, std::shared_ptr<int> tileY) {
    if (!_graphics)
        return false;
    ZappyTypes::Vector3 cameraPos = _graphics->GetCameraPosition();
    ZappyTypes::Vector3 rayDirection = _graphics->ScreenToWorldRay(screenPos);
    ZappyTypes::Vector3 planePoint = {0.0f, 0.0f, 0.0f};
    ZappyTypes::Vector3 planeNormal = {0.0f, 1.0f, 0.0f};
    ZappyTypes::Vector3 intersectionPoint;
    if (_graphics->RayPlaneIntersection(cameraPos, rayDirection,
        planePoint, planeNormal, intersectionPoint))
        return worldToTileCoordinates(intersectionPoint, tileX, tileY);
    return false;
}

float GameLoop::calculateRayToPointDistance(ZappyTypes::Vector3 rayOrigin, ZappyTypes::Vector3 rayDirection, ZappyTypes::Vector3 point) {
    ZappyTypes::Vector3 rayToPoint = {point.x - rayOrigin.x, point.y - rayOrigin.y, point.z - rayOrigin.z};
    float dotProduct = rayToPoint.x * rayDirection.x + rayToPoint.y * rayDirection.y + rayToPoint.z * rayDirection.z;
    ZappyTypes::Vector3 closestPointOnRay = {rayOrigin.x + dotProduct * rayDirection.x, rayOrigin.y + dotProduct * rayDirection.y, rayOrigin.z + dotProduct * rayDirection.z};
    ZappyTypes::Vector3 diff = {point.x - closestPointOnRay.x, point.y - closestPointOnRay.y, point.z - closestPointOnRay.z};
    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

ZappyTypes::Vector3 GameLoop::calculatePlayerWorldPosition(int playerX, int playerY, int playerIndex, int totalPlayers) {
    float tileSize = 1.0f;
    float spacing = 1.5f;

    if (_mapWidth > 20 || _mapHeight > 20) {
        tileSize = 10.0f / std::max(_mapWidth, _mapHeight);
        spacing = tileSize * 0.1f;
    }
    float mapCenterX = _mapWidth / 2.0f;
    float mapCenterY = _mapHeight / 2.0f;
    ZappyTypes::Vector3 playerPos = {(playerX - mapCenterX + 0.5f) * (tileSize + spacing), 0.55f, (playerY - mapCenterY + 0.5f) * (tileSize + spacing)};
    if (totalPlayers > 1) {
        float stackHeight = 1.1f;
        playerPos.y += playerIndex * stackHeight;
    }
    return playerPos;
}

bool GameLoop::performPlayerSelection(ZappyTypes::Vector2 screenPos, std::shared_ptr<int> playerId) {
    if (!_gameController || !_graphics)
        return false;
    auto gameState = _gameController->getGameState();
    if (!gameState || !gameState->isMapInitialized())
        return false;
    ZappyTypes::Vector3 cameraPos = _graphics->GetCameraPosition();
    ZappyTypes::Vector3 rayDirection = _graphics->ScreenToWorldRay(screenPos);
    float minDistance = std::numeric_limits<float>::max();
    int closestPlayerId = -1;
    bool foundPlayer = false;
    for (int y = 0; y < _mapHeight; ++y) {
        for (int x = 0; x < _mapWidth; ++x) {
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
    _mapWidth = width;
    _mapHeight = height;
    updateCameraForMapSize();
}

void GameLoop::onTileChanged(int x, int y, const std::shared_ptr<const ITile>& tile) {
    if (!_gameController || !tile)
        return;
    auto gameState = _gameController->getGameState();
    if (gameState && gameState->isMapInitialized()) {
        auto mutableGameState = std::const_pointer_cast<GameState>(gameState);
        const auto& res = tile->getResources();
        mutableGameState->updateTileResources(x, y, res[0], res[1], res[2], res[3], res[4], res[5], res[6]);
    }
}
