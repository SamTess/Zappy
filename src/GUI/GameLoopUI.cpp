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
 * Met à jour les données de jeu et le GameState
 */
void GameLoop::updateGameData() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    // Mise à jour du temps de jeu
    m_gameTime += deltaTime;
    m_gameTick++;
    
    // Gestion des timers d'incantation
    for (auto it = playerIncantationTimers.begin(); it != playerIncantationTimers.end(); ) {
        int playerId = it->first;
        float& timer = it->second;
        timer -= deltaTime;
        if (timer <= 0.0f) {
            // Mettre à jour le joueur via le GameState
            if (m_gameController) {
                auto gameState = m_gameController->getGameState();
                auto player = gameState->getPlayerInfo(playerId);
                if (player) {
                    // Cette logique devrait être dans le GameController
                    // À implémenter: marquage de fin d'incantation pour ce joueur
                }
            }
            it = playerIncantationTimers.erase(it);
        } else {
            ++it;
        }
    }
    
    // Mise à jour du GameController et GameState
    if (m_gameController) {
        m_gameController->updateBroadcasts(deltaTime);
        auto gameState = m_gameController->getGameState();
        if (gameState && gameState->isMapInitialized()) {
            // Mise à jour des informations de carte
            m_mapWidth = gameState->getMapWidth();
            m_mapHeight = gameState->getMapHeight();
            m_frequency = gameState->getTimeUnit();
        }
    }
    
    // Si le joueur a cliqué sur une case, vérifier les interactions
    if (!m_userInterface->hasHandledMouseEvent() && 
        !m_userInterface->isMouseOverUI() && 
        m_graphics->IsMouseButtonPressed(0)) {
        ZappyTypes::Vector2 mousePos = m_graphics->GetMousePosition();
        
        // Calcul des coordonnées de la case cliquée (à adapter selon votre système de rendu)
        int tileX = static_cast<int>(mousePos.x / 32) % m_mapWidth;
        int tileY = static_cast<int>(mousePos.y / 32) % m_mapHeight;
        
        handleTileSelection(tileX, tileY);
    }    // Si le joueur a cliqué sur une case, vérifier les interactions
    if (!m_userInterface->hasHandledMouseEvent() && 
        !m_userInterface->isMouseOverUI() && 
        m_graphics->IsMouseButtonPressed(0)) {
        ZappyTypes::Vector2 mousePos = m_graphics->GetMousePosition();
        
        // Calcul des coordonnées de la case cliquée (à adapter selon votre système de rendu)
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
    
    // Au lieu de créer une tuile, on en récupère une du GameState
    if (m_gameController) {
        auto gameState = m_gameController->getGameState();
        auto tile = gameState->getTile(x, y);
        // La tuile est déjà dans le GameState, pas besoin de la créer
        
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
    // Les dimensions sont maintenant stockées directement dans m_mapWidth et m_mapHeight
    updateCameraForMapSize();
}

void GameLoop::onTileChanged(int x, int y, const TileData& tileData) {
    if (m_gameController) {
        // Utilise la méthode du GameState pour mettre à jour les ressources de la tuile
        auto gameState = m_gameController->getGameState();
        if (gameState && gameState->isMapInitialized()) {
            // Utilise const_cast pour accéder aux méthodes non-const du GameState
            // C'est safe car nous savons que gameState pointe vers l'instance gérée par GameController
            auto mutableGameState = std::const_pointer_cast<GameState>(gameState);
            mutableGameState->updateTileResources(x, y,
                tileData.resources[0],  // food
                tileData.resources[1],  // linemate
                tileData.resources[2],  // deraumere
                tileData.resources[3],  // sibur
                tileData.resources[4],  // mendiane
                tileData.resources[5],  // phiras
                tileData.resources[6]   // thystame
            );
        }
    }
    
    if (m_selectedTile.selected && m_selectedTile.x == x && m_selectedTile.y == y) {
        // Rafraîchir les informations affichées si nécessaire
    }
}

// Les méthodes de mise à jour ont été supprimées car nous utilisons
// directement les entités du GameState au lieu des structs de GameData
