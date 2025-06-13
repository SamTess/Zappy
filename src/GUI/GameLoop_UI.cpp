/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameLoop UI Implementation
*/

#include "GameLoop.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <cstdlib>
#include <map>

// Générateur de nombres aléatoires global
static std::mt19937 rng(std::random_device{}());

// Timers pour les incantations des joueurs
static std::map<int, float> playerIncantationTimers;

/**
 * Met à jour les données de jeu et rafraîchit l'interface utilisateur
 */
void GameLoop::updateGameData()
{
    // Dans un contexte réel, ces données seraient mises à jour à partir du réseau
    // Pour cet exemple, nous simulons quelques données

    // Mise à jour du temps et de la fréquence
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;

    m_gameData.time += deltaTime;
    m_gameData.tick++;
    
    // Gérer les timers d'incantation
    for (auto it = playerIncantationTimers.begin(); it != playerIncantationTimers.end(); ) {
        int playerId = it->first;
        float& timer = it->second;
        
        timer -= deltaTime;
        
        if (timer <= 0.0f) {
            // Trouver le joueur et désactiver l'incantation
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

    // Simuler des événements aléatoires pour tester l'interface
    simulateRandomEvents(deltaTime);

    // Vérifier les entrées utilisateur pour la sélection de cases
    if (m_graphics->IsMouseButtonPressed(0)) { // Clic gauche
        // Utilisation des coordonnées de la souris pour la sélection de tuile
        ZappyTypes::Vector2 mousePos = m_graphics->GetMousePosition();
        // Calcul simplifié pour exemple - normalement on ferait une intersection rayon-plan
        // et une conversion en coordonnées de la grille
        int tileX = static_cast<int>(mousePos.x / 32) % m_gameData.mapWidth;
        int tileY = static_cast<int>(mousePos.y / 32) % m_gameData.mapHeight;
        handleTileSelection(tileX, tileY);
    }

    // Vérifier les touches pour les commandes de test
    checkTestCommands();

    // Transmettre les données mises à jour à l'interface
    m_userInterface->updateData(m_gameData);
}

/**
 * Simule des événements aléatoires pour tester l'interface
 */
void GameLoop::simulateRandomEvents(float deltaTime)
{
    static float eventTimer = 0.0f;
    static float broadcastTimer = 0.0f;
    static float playerMoveTimer = 0.0f;
    
    // Générer un événement aléatoire toutes les 3 secondes
    eventTimer += deltaTime;
    if (eventTimer > 3.0f) {
        eventTimer = 0.0f;
        
        // Types d'événements possibles
        std::uniform_int_distribution<int> eventTypeDist(0, 3);
        int eventType = eventTypeDist(rng);
        
        switch (eventType) {
            case 0: // Nouveau joueur
                if (m_gameData.players.size() < 20) {
                    GUI::Player newPlayer;
                    newPlayer.id = m_gameData.players.size() + 10; // ID unique
                    newPlayer.team = m_gameData.teams[rand() % m_gameData.teams.size()].name;
                    newPlayer.x = rand() % m_gameData.mapWidth;
                    newPlayer.y = rand() % m_gameData.mapHeight;
                    newPlayer.orientation = (rand() % 4) + 1;
                    newPlayer.level = (rand() % 7) + 1;
                    m_gameData.players.push_back(newPlayer);
                    
                    std::stringstream ss;
                    ss << "Nouveau joueur connecté (ID: " << newPlayer.id << ", Équipe: " << newPlayer.team << ")";
                    m_userInterface->addLogMessage(ss.str());
                }
                break;
            case 1: // Nouvelle ressource sur une case
                {
                    int x = rand() % m_gameData.mapWidth;
                    int y = rand() % m_gameData.mapHeight;
                    
                    // Créer un TileData pour la mise à jour
                    TileData tileData;
                    tileData.resources[0] = rand() % 5; // Food
                    tileData.resources[1] = rand() % 3; // Linemate
                    tileData.resources[2] = rand() % 2; // Deraumere
                    tileData.resources[3] = rand() % 2; // Sibur
                    tileData.resources[4] = rand() % 1; // Mendiane
                    tileData.resources[5] = rand() % 1; // Phiras
                    tileData.resources[6] = 0;          // Thystame (rare)
                    
                    onTileChanged(x, y, tileData);
                    
                    std::stringstream ss;
                    ss << "Ressources mises à jour sur la case (" << x << ", " << y << ")";
                    m_userInterface->addLogMessage(ss.str());
                }
                break;
            case 2: // Changement de niveau d'un joueur
                if (!m_gameData.players.empty()) {
                    int playerIndex = rand() % m_gameData.players.size();
                    GUI::Player& player = m_gameData.players[playerIndex];
                    
                    if (player.level < 8) {
                        player.level++;
                        player.isIncantating = true; // Simuler une incantation
                        
                        std::stringstream ss;
                        ss << "Joueur " << player.id << " de l'équipe " << player.team 
                           << " monte au niveau " << player.level;
                        m_userInterface->addLogMessage(ss.str());
                        m_userInterface->addBroadcast(player.team, "Je monte au niveau " + std::to_string(player.level) + " !");
                        
                        // Initialiser le timer pour cette incantation (sera géré dans updateGameData)
                        playerIncantationTimers[player.id] = 1.0f; // 1 seconde
                    }
                }
                break;
            case 3: // Mort d'un joueur (manque de nourriture)
                if (m_gameData.players.size() > 2) { // Garder au moins 2 joueurs
                    int playerIndex = rand() % m_gameData.players.size();
                    GUI::Player player = m_gameData.players[playerIndex];
                    
                    std::stringstream ss;
                    ss << "Joueur " << player.id << " de l'équipe " << player.team 
                       << " est mort de faim";
                    m_userInterface->addLogMessage(ss.str());
                    
                    // Supprimer le joueur
                    m_gameData.players.erase(m_gameData.players.begin() + playerIndex);
                }
                break;
        }
    }
    
    // Simuler des broadcasts aléatoires
    broadcastTimer += deltaTime;
    if (broadcastTimer > 5.0f) {
        broadcastTimer = 0.0f;
        
        if (!m_gameData.players.empty() && !m_gameData.teams.empty()) {
            // Messages possibles
            std::vector<std::string> messages = {
                "J'ai besoin de nourriture !",
                "Ressources trouvées ici !",
                "Rencontre au centre de la carte",
                "Besoin d'aide pour incantation",
                "Attention danger ici !",
                "Rejoignez-moi pour monter de niveau",
                "Je vais bien, ne vous inquiétez pas",
                "Qui a vu des pierres précieuses ?"
            };
            
            std::string teamName = m_gameData.teams[rand() % m_gameData.teams.size()].name;
            std::string message = messages[rand() % messages.size()];
            
            m_userInterface->addBroadcast(teamName, message);
        }
    }
    
    // Simuler le mouvement des joueurs
    playerMoveTimer += deltaTime;
    if (playerMoveTimer > 0.5f) {
        playerMoveTimer = 0.0f;
        
        // Déplacer quelques joueurs aléatoirement
        for (auto& player : m_gameData.players) {
            // 20% de chance de bouger
            if (rand() % 5 == 0) {
                // Direction aléatoire
                int direction = rand() % 4;
                switch(direction) {
                    case 0: // nord
                        player.y = (player.y - 1 + m_gameData.mapHeight) % m_gameData.mapHeight;
                        player.orientation = 1;
                        break;
                    case 1: // est
                        player.x = (player.x + 1) % m_gameData.mapWidth;
                        player.orientation = 2;
                        break;
                    case 2: // sud
                        player.y = (player.y + 1) % m_gameData.mapHeight;
                        player.orientation = 3;
                        break;
                    case 3: // ouest
                        player.x = (player.x - 1 + m_gameData.mapWidth) % m_gameData.mapWidth;
                        player.orientation = 4;
                        break;
                }
            }
        }
    }
}

/**
 * Vérifie les touches clavier pour les commandes de test
 */
void GameLoop::checkTestCommands()
{
    // Touche 'L' pour ajouter un log de test
    if (m_graphics->IsKeyPressed(ZappyTypes::Z_KEY_L)) {
        m_userInterface->addLogMessage("Ceci est un message de log de test");
    }
    
    // Touche 'B' pour ajouter un broadcast de test
    if (m_graphics->IsKeyPressed(ZappyTypes::Z_KEY_B)) {
        if (!m_gameData.teams.empty()) {
            std::string team = m_gameData.teams[rand() % m_gameData.teams.size()].name;
            m_userInterface->addBroadcast(team, "Broadcast de test depuis touche B");
        }
    }
    
    // Touche 'M' pour modifier la taille de la map (test)
    if (m_graphics->IsKeyPressed(ZappyTypes::Z_KEY_M)) {
        int newWidth = m_mapWidth + 1;
        int newHeight = m_mapHeight + 1;
        if (newWidth > 30) newWidth = 10;
        if (newHeight > 30) newHeight = 10;
        
        onMapSizeChanged(newWidth, newHeight);
    }
}

/**
 * Gère la sélection d'une case par l'utilisateur
 */
void GameLoop::handleTileSelection(int x, int y)
{
    if (x < 0 || y < 0 || x >= m_gameData.mapWidth || y >= m_gameData.mapHeight)
        return;

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
void GameLoop::handleViewModeChange(int mode)
{
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
    
    // Mettre à jour les données de jeu également
    m_gameData.mapWidth = width;
    m_gameData.mapHeight = height;
    
    std::stringstream message;
    message << "Dimensions de la map modifiées : " << width << "x" << height;
    m_userInterface->addLogMessage(message.str());
}

void GameLoop::onTileChanged(int x, int y, const TileData& tileData) {
    // Rechercher ou créer la case
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
    
    // Mettre à jour les ressources sur la case en utilisant le tableau resources
    targetTile->food = tileData.resources[0]; // ResourceType::FOOD
    targetTile->linemate = tileData.resources[1]; // ResourceType::LINEMATE
    targetTile->deraumere = tileData.resources[2]; // ResourceType::DERAUMERE
    targetTile->sibur = tileData.resources[3]; // ResourceType::SIBUR
    targetTile->mendiane = tileData.resources[4]; // ResourceType::MENDIANE
    targetTile->phiras = tileData.resources[5]; // ResourceType::PHIRAS
    targetTile->thystame = tileData.resources[6]; // ResourceType::THYSTAME
    
    // Si c'est la case actuellement sélectionnée, rafraîchir l'affichage
    if (m_selectedTile.selected && m_selectedTile.x == x && m_selectedTile.y == y) {
        // Rafraîchir les informations affichées
    }
}
