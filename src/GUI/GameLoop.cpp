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
#include "renderer/ParticleSystem.hpp"
#include "renderer/EjectionAnimationManager.hpp"
#include "renderer/DeathAnimationManager.hpp"

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
    m_graphics->PlayMusic("assets/music/music.mp3");
    m_graphics->SetMusicVolume(0.5f);
    m_gameController->setGraphics(m_graphics);
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
    m_modelManagerAdapter = Zappy::ModelManagerAdapter::createShared();
    m_modelManagerAdapter->setGraphicsLib(m_graphics);
    m_mapRenderer = std::make_shared<Zappy::MapRenderer>(m_graphics, m_gameController->getGameState(), m_modelManagerAdapter);
    m_userInterface = std::make_shared<GUI::UserInterface>(m_gui);
    m_userInterface->initialize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    m_userInterface->setNetworkManager(m_networkManager);
}

int GameLoop::run() {
    if (!m_graphics || !m_gui || !m_renderer || !m_camera || !m_uiRenderer)
        return 84;
    while (!m_graphics->WindowShouldClose()) {
        bool uiHandledMouse = m_userInterface->handleMouseEvents();
        bool mouseOverUI = m_userInterface->isMouseOverUI();
        m_camera->update(m_graphics, uiHandledMouse, mouseOverUI);
        m_graphics->BeginDrawing();
        if (m_renderer) {
            m_renderer->renderSkybox(m_graphics);
        }
        m_graphics->ClearBackground({32, 32, 64, 255});
        m_graphics->BeginCamera3D();
        m_mapRenderer->render();
        Zappy::ParticleSystem::getInstance().render(m_graphics);
        Zappy::EjectionAnimationManager::getInstance().render(m_graphics);
        Zappy::DeathAnimationManager::getInstance().render(m_graphics);
        m_graphics->EndCamera3D();
        updateGameData();
        if (m_gameController) {
            auto gameState = m_gameController->getGameState();
            m_userInterface->updateDataFromGameState(gameState, m_mapWidth, m_mapHeight,
                                                   m_gameTime, m_frequency, m_gameTick);
        }
        m_userInterface->render();
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
    if (m_gameController && m_graphics) {
        m_gameController->setGraphics(m_graphics);
    }
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
    m_graphics->UpdateMusic();
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
