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
    auto& modelManager = ModelManager::getInstance();
    m_cubeModelId = modelManager.loadModel("assets/models/Cube/cube.obj", "assets/models/Cube/cube_diffuse.png");
    return true;
}

void GameLoop::setupComponents() {
    m_camera = std::make_shared<CameraController>();
    m_camera->init(m_graphics);
    m_camera->setMapDimensions(20, 20);
    m_uiRenderer = std::make_shared<UIRenderer>();
    if (!m_context) {
        m_context = std::make_shared<GraphicalContext>();
    }
    m_modelManagerAdapter = Zappy::ModelManagerAdapter::createShared();
    m_modelManagerAdapter->setGraphicsLib(m_graphics);
    m_mapRenderer = std::make_shared<Zappy::MapRenderer>(m_graphics, m_context, m_modelManagerAdapter);
    m_mapRenderer->initialize();
    m_mapRendererObserver = std::make_shared<Zappy::MapRendererObserver>(m_mapRenderer);
    m_context->addObserver(m_mapRendererObserver);
}

int GameLoop::run() {
    if (!m_graphics || !m_gui || !m_renderer || !m_camera || !m_uiRenderer || !m_mapRenderer) {
        std::cerr << "Game components not initialized properly." << std::endl;
        return 84;
    }

    while (!m_graphics->WindowShouldClose()) {
        m_camera->update(m_graphics);
        m_graphics->BeginDrawing();
        m_graphics->ClearBackground({32, 32, 64, 255});
        m_graphics->BeginCamera3D();
        if (m_mapRenderer)
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

void GameLoop::setGraphicalContext(std::shared_ptr<GraphicalContext> context) {
    if (m_context && m_mapRendererObserver) {
        m_context->removeObserver(m_mapRendererObserver);
    }
    m_context = context;
    std::cout << "GraphicalContext set in GameLoop" << std::endl;
    if (m_mapRenderer && m_mapRendererObserver) {
        m_context->addObserver(m_mapRendererObserver);
        std::cout << "MapRenderer observer reconnected to new GraphicalContext" << std::endl;
    }
}

