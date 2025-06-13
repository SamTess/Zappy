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
#include <algorithm>
#include "Constants.hpp"
#include "textureManager/ModelManager.hpp"
#include "textureManager/ModelManagerAdapter.hpp"
GameLoop::GameLoop(std::shared_ptr<NetworkManager> networkManager)
    : m_host("localhost"), m_port(4242), m_networkManager(networkManager) {
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

    modelManager.loadModel("assets/models/baby_Tripy_Trophy.glb");
    modelManager.loadModel("assets/models/baby_Espressona_Signora.glb");
    modelManager.loadModel("assets/models/baby_Frigo_Camelo.glb");
    modelManager.loadModel("assets/models/baby_Garamaran.glb");
    modelManager.loadModel("assets/models/baby_La_Vaca_Saturno_Saturnita.glb");
    modelManager.loadModel("assets/models/baby_TRALALERO_TRALALA.glb");
    modelManager.loadModel("assets/models/Baby_Trulimero_Trulicina.glb");
    modelManager.loadModel("assets/models/baby_tung_tung_tung_sahur.glb");
    modelManager.loadModel("assets/models/island.glb");
    modelManager.loadModel("assets/models/labubu.glb");

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
    if (!m_graphics || !m_gui || !m_renderer || !m_camera || !m_uiRenderer)
        return 84;
    while (!m_graphics->WindowShouldClose()) {
        m_camera->update(m_graphics);
        m_graphics->BeginDrawing();
        if (m_renderer) {
            m_renderer->renderSkybox(m_graphics);
        }
        m_graphics->ClearBackground({32, 32, 64, 255});
        m_graphics->BeginCamera3D();
        m_mapRenderer->render();
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
    return 0;
}

void GameLoop::setServerInfo(const std::string& host, int port) {
    m_host = host;
    m_port = port;
}

void GameLoop::setGameController(std::shared_ptr<GameController> controller) {
    m_gameController = controller;
    if (m_gameController && m_graphics && m_modelManagerAdapter) {
        auto gameState = m_gameController->getGameState();
        m_mapRenderer = std::make_shared<Zappy::MapRenderer>(m_graphics, gameState, m_modelManagerAdapter);
        m_mapRenderer->initialize();
        if (gameState->isMapInitialized()) {
            updateCameraForMapSize();
        }
    }
}

void GameLoop::updateCameraForMapSize() {
    if (!m_gameController || !m_camera)
        return;
    auto gameState = m_gameController->getGameState();
    if (!gameState->isMapInitialized())
        return;
    int mapWidth = gameState->getMapWidth();
    int mapHeight = gameState->getMapHeight();
    m_camera->setMapDimensions(mapWidth, mapHeight);
    float tileSize = 1.0f;
    float spacing = 0.1f;
    if (mapWidth > 20 || mapHeight > 20) {
        tileSize = 10.0f / std::max(mapWidth, mapHeight);
        spacing = tileSize * 0.1f;
    }
    float mapExtentX = mapWidth * (tileSize + spacing);
    float mapExtentZ = mapHeight * (tileSize + spacing);
    float maxExtent = std::max(mapExtentX, mapExtentZ);
    float cameraDistance = maxExtent * 1.5f;
    if (cameraDistance < 10.0f)
        cameraDistance = 10.0f;
    if (cameraDistance > 50.0f)
        cameraDistance = 50.0f;
    m_camera->reset();
    m_camera->distance() = cameraDistance;
}

void GameLoop::setSkyboxTexture(const std::string& texturePath) {
    if (m_renderer) {
        m_renderer->setSkyboxTexture(texturePath);
    }
}

bool GameLoop::isSkyboxLoaded() const {
    return m_renderer && m_renderer->isSkyboxLoaded();
}

