/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameLoop
*/

#include "GameLoop.hpp"
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <chrono>
#include "Constants.hpp"
#include "textureManager/ModelManager.hpp"

GameLoop::GameLoop()
    : m_host("localhost"), m_port(4242) {
}

bool GameLoop::init() {
    if (!loadLibraries())
        return false;
    initializeManagers();
    if (!loadModels())
        return false;
    setupComponents();
    return true;
}

bool GameLoop::loadLibraries() {
    auto& libraryManager = LibraryManager::getInstance();
    if (!libraryManager.loadGraphicsLib("plugins/libraylibcpp.so")) {
        std::cerr << "Erreur de chargement de la bibliothèque graphique: " << libraryManager.getLastError() << std::endl;
        return false;
    }
    if (!libraryManager.loadGuiLib("plugins/libraygui.so")) {
        std::cerr << "Erreur de chargement de la bibliothèque GUI: " << libraryManager.getLastError() << std::endl;
        return false;
    }
    m_graphics = libraryManager.getGraphicsLibPtr();
    m_gui = libraryManager.getGuiLibPtr();
    return true;
}

void GameLoop::initializeManagers() {
    auto& textureManager = TextureManager::getInstance();
    textureManager.setGraphicsLib(m_graphics);

    auto& modelManager = ModelManager::getInstance();
    modelManager.setGraphicsLib(m_graphics);

    m_renderer = std::make_shared<Renderer>();
    m_renderer->init(m_graphics);
}

bool GameLoop::loadModels() {
    auto& modelManager = ModelManager::getInstance();
    m_cubeModelId = modelManager.loadModel("assets/models/Cube/cube.obj", "assets/models/Cube/cube_diffuse.png");
    return true;
}

void GameLoop::setupComponents() {
    m_camera = std::make_shared<CameraController>();
    m_camera->init(m_graphics);
    m_camera->setMapDimensions(20, 20);
    m_uiRenderer = std::make_shared<UIRenderer>();
    
    // Initialisation de l'interface utilisateur
    m_userInterface = std::make_shared<GUI::UserInterface>(m_gui);
    m_userInterface->initialize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    // Configuration initiale des données de jeu
    m_gameData.mapWidth = m_mapWidth;
    m_gameData.mapHeight = m_mapHeight;
    
    // Ajouter quelques données de test
    m_userInterface->addLogMessage("Interface utilisateur initialisée");
    
    // Ajout de données de test pour la démonstration
    // Équipes
    GUI::Team team1 = {"Team1", 5};
    GUI::Team team2 = {"Team2", 3};
    m_gameData.teams.push_back(team1);
    m_gameData.teams.push_back(team2);
    
    // Joueurs
    GUI::Player player1;
    player1.id = 1;
    player1.team = "Team1";
    player1.x = 3;
    player1.y = 4;
    player1.orientation = 1;
    player1.level = 2;
    player1.inventory.food = 10;
    player1.inventory.linemate = 2;
    m_gameData.players.push_back(player1);
    
    GUI::Player player2;
    player2.id = 2;
    player2.team = "Team2";
    player2.x = 8;
    player2.y = 7;
    player2.orientation = 3;
    player2.level = 1;
    player2.inventory.food = 5;
    m_gameData.players.push_back(player2);
    
    // Ressources sur des cases
    for (int i = 0; i < 10; i++) {
        GUI::Tile tile;
        tile.x = rand() % m_mapWidth;
        tile.y = rand() % m_mapHeight;
        tile.food = rand() % 5;
        tile.linemate = rand() % 3;
        tile.deraumere = rand() % 2;
        tile.sibur = rand() % 2;
        m_gameData.tiles.push_back(tile);
    }
    
    // Quelques logs et broadcasts de test
    m_userInterface->addLogMessage("Connexion au serveur établie");
    m_userInterface->addLogMessage("Map de dimensions: " + std::to_string(m_mapWidth) + "x" + std::to_string(m_mapHeight));
    m_userInterface->addBroadcast("Team1", "Je cherche de la nourriture!");
    m_userInterface->addBroadcast("Team2", "Rendez-vous au point x:8 y:5");
}

int GameLoop::run() {
    if (!m_graphics || !m_gui || !m_renderer || !m_camera || !m_uiRenderer) {
        std::cerr << "Game components not initialized properly." << std::endl;
        return 84;
    }

    while (!m_graphics->WindowShouldClose()) {
        m_camera->update(m_graphics);
        m_graphics->BeginDrawing();
        m_graphics->ClearBackground({32, 32, 64, 255});
        m_graphics->BeginCamera3D();
        m_graphics->DrawPlane({0.0f, 0.0f, 0.0f}, {20.0f, 20.0f}, {200, 200, 200, 255});
        renderCube();
        m_graphics->EndCamera3D();
        
        // Mise à jour des données de jeu
        updateGameData();
        
        // Rendu de l'interface utilisateur
        m_userInterface->render();
        
        // Rendu des éléments UI additionnels si nécessaire
        m_uiRenderer->renderUI(m_graphics, m_gui, m_camera);
        m_graphics->EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    m_graphics->CloseWindow();
    return 0;
}

void GameLoop::setServerInfo(const std::string& host, int port) {
    m_host = host;
    m_port = port;
}

void GameLoop::renderCube() {
    auto& modelManager = ModelManager::getInstance();
    if (m_cubeModelId != -1) {
        ZappyTypes::Vector3 cubePosition = {2.0f, 0.0f, 2.0f};
        ZappyTypes::Color cubeColor = {255, 255, 255, 255};
        modelManager.drawModel(m_cubeModelId, cubePosition, cubeColor);
    } else {
        std::cerr << "Modèle cube.obj non trouvé." << std::endl;
    }
}

// Les implémentations des méthodes onMapSizeChanged et onTileChanged
// sont définies dans GameLoop_UI.cpp
