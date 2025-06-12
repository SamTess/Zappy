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

// Fonction qui teste le système de chargement et de cache des modèles/textures
bool GameLoop::loadModels() {
    auto& modelManager = ModelManager::getInstance();
    m_cubeModelId = modelManager.loadModel("assets/models/Cube/cube.obj", "assets/models/Cube/cube_diffuse.png");
    return true;
}

void GameLoop::setupComponents() {
    m_camera = std::make_shared<CameraController>();
    m_camera->init(m_graphics);
    // Définir les dimensions de la carte pour limiter les mouvements de la caméra
    m_camera->setMapDimensions(20, 20);  // Valeurs par défaut ou à mettre à jour plus tard
    m_uiRenderer = std::make_shared<UIRenderer>();
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
