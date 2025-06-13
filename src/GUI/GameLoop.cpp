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
#include "textureManager/ModelManagerAdapter.hpp"

GameLoop::GameLoop()
    : m_host("localhost"), m_port(4242) {
}

bool GameLoop::init() {
    if (!loadLibraries())
        return false;
    initializeManagers();
    // if (!loadModels())
    //     return false;
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
    // auto& modelManager = ModelManager::getInstance();
    // m_cubeModelId = modelManager.loadModel("assets/models/Cube/cube.obj", "assets/models/Cube/cube_diffuse.png");
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
}

int GameLoop::run() {
    if (!m_graphics || !m_gui || !m_renderer || !m_camera || !m_uiRenderer)
        return 84;
    while (!m_graphics->WindowShouldClose()) {
        m_camera->update(m_graphics);
        m_graphics->BeginDrawing();
        m_graphics->ClearBackground({32, 32, 64, 255});
        m_graphics->BeginCamera3D();
        m_mapRenderer->render();
        m_graphics->EndCamera3D();
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

