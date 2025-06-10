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
    auto& textureManager = TextureManager::getInstance();
    
    std::cout << "\n=== Test du système de cache des textures et modèles ===\n" << std::endl;
    
    // Chargement initial des textures individuelles pour tester le cache
    std::cout << "Chargement direct de textures via TextureManager:" << std::endl;
    int girlTextureId = textureManager.loadTexture("assets/models/14-girl-obj/tEXTURE/girl.png");
    int castleTextureId = textureManager.loadTexture("assets/models/castle_diffuse.png");
    int cubeTextureId = textureManager.loadTexture("assets/models/Cube/cube_diffuse.png");
    
    // Rechargement des mêmes textures (devrait retourner les mêmes IDs)
    std::cout << "\nRechargement des mêmes textures (devrait réutiliser les instances existantes):" << std::endl;
    int girlTextureId2 = textureManager.loadTexture("assets/models/14-girl-obj/tEXTURE/girl.png");
    int castleTextureId2 = textureManager.loadTexture("assets/models/castle_diffuse.png");
    int cubeTextureId2 = textureManager.loadTexture("assets/models/Cube/cube_diffuse.png");
    
    std::cout << "\nVérification des IDs de textures:" << std::endl;
    std::cout << "Girl texture: Premier chargement ID = " << girlTextureId << ", Second chargement ID = " << girlTextureId2 << std::endl;
    std::cout << "Castle texture: Premier chargement ID = " << castleTextureId << ", Second chargement ID = " << castleTextureId2 << std::endl;
    std::cout << "Cube texture: Premier chargement ID = " << cubeTextureId << ", Second chargement ID = " << cubeTextureId2 << std::endl;
    
    std::cout << "\nChargement des modèles (avec textures via ModelManager):" << std::endl;
    
    // Chargement du modèle girl
    int girlModelId = modelManager.loadModel("assets/models/14-girl-obj/girl.obj");
    if (girlModelId != -1) {
        std::cout << "Modèle girl.obj chargé avec succès (ID: " << girlModelId << ")" << std::endl;
        m_girlModelId = girlModelId;
    } else {
        std::cerr << "Impossible de charger le modèle girl.obj" << std::endl;
    }
    
    // Chargement du modèle castle avec texture
    int castleModelId = modelManager.loadModel("assets/models/castle.obj", "assets/models/castle_diffuse.png");
    if (castleModelId != -1) {
        std::cout << "Modèle castle.obj avec texture chargé avec succès (ID: " << castleModelId << ")" << std::endl;
        m_castleModelId = castleModelId;
    } else {
        std::cerr << "Impossible de charger le modèle castle.obj avec sa texture" << std::endl;
    }
    
    // Premier chargement du cube
    int cubeModelId = modelManager.loadModel("assets/models/Cube/cube.obj", "assets/models/Cube/cube_diffuse.png");
    if (cubeModelId != -1) {
        std::cout << "Modèle cube.obj avec texture chargé avec succès (ID: " << cubeModelId << ")" << std::endl;
        m_cubeModelId = cubeModelId;
    } else {
        std::cerr << "Impossible de charger le modèle cube.obj avec sa texture" << std::endl;
    }
    
    // Deuxième chargement du cube - devrait réutiliser le même modèle
    std::cout << "\nRechargement du même modèle (devrait réutiliser l'instance existante):" << std::endl;
    int cubeModelIdBis = modelManager.loadModel("assets/models/Cube/cube.obj", "assets/models/Cube/cube_diffuse.png");
    if (cubeModelIdBis != -1) {
        std::cout << "Modèle cube.obj rechargé (ID: " << cubeModelIdBis << ")" << std::endl;
        m_cubeModelIdBis = cubeModelIdBis;
        
        if (cubeModelId == cubeModelIdBis) {
            std::cout << "SUCCÈS: Les deux chargements du même modèle ont retourné le même ID." << std::endl;
        } else {
            std::cerr << "ÉCHEC: Les deux chargements ont retourné des IDs différents!" << std::endl;
        }
    } else {
        std::cerr << "Impossible de recharger le modèle cube.obj avec sa texture" << std::endl;
    }
    
    std::cout << "\nStatistiques du cache:" << std::endl;
    std::cout << "Nombre total de textures en cache: " << textureManager.getTextureCount() << std::endl;
    std::cout << "Nombre total de modèles en cache: " << modelManager.getModelCount() << std::endl;
    std::cout << "\n=== Fin des tests du système de cache ===\n" << std::endl;
    
    return true;
}

void GameLoop::setupComponents() {
    m_camera = std::make_shared<CameraController>();
    m_camera->init(m_graphics);
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
        renderGirl();
        renderCube();
        renderCastle();
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

void GameLoop::renderGirl() {
    auto& modelManager = ModelManager::getInstance();
    if (m_girlModelId != -1) {
        ZappyTypes::Vector3 girlPosition = {0.0f, 0.0f, 4.0f};
        modelManager.drawModel(m_girlModelId, girlPosition, {255, 255, 255, 255});
    }
}

void GameLoop::renderCastle() {
    auto& modelManager = ModelManager::getInstance();
    if (m_castleModelId != -1) {
        ZappyTypes::Vector3 castlePosition = {0.0f, 0.0f, 0.0f};
        modelManager.drawModel(m_castleModelId, castlePosition, {255, 255, 255, 255});
    }
}

void GameLoop::renderCube() {
    auto& modelManager = ModelManager::getInstance();
    if (m_cubeModelId != -1) {
        ZappyTypes::Vector3 cubePosition = {2.0f, 0.0f, 2.0f};
        ZappyTypes::Color cubeColor = {255, 255, 255, 255};
        modelManager.drawModel(m_cubeModelId, cubePosition, cubeColor);
        if (m_cubeModelIdBis != -1) {
            ZappyTypes::Vector3 cubeBisPosition = {-2.0f, 0.0f, -2.0f};
            modelManager.drawModel(m_cubeModelIdBis, cubeBisPosition, {255, 255, 255, 255});
        }
    } else {
        std::cerr << "Modèle cube.obj non trouvé." << std::endl;
    }
}
