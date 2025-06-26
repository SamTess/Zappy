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
#include "ui/windows/timeInfo/TimeInfoWindow.hpp"
#include "renderer/ParticleSystem.hpp"
#include "renderer/EjectionAnimationManager.hpp"
#include "renderer/DeathAnimationManager.hpp"

GameLoop::GameLoop(std::shared_ptr<NetworkManager> networkManager)
    : _host("localhost"), _port(4242), _networkManager(networkManager) {
}

bool GameLoop::init() {
    if (!loadLibraries())
        return false;
    initializeManagers();
    if (!loadModels())
        return false;
    setupComponents();
    _graphics->PlayMusic("assets/music/music.mp3");
    _graphics->SetMusicVolume(0.5f);
    if (_gameController)
        _gameController->setGraphics(_graphics);
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
    _graphics = libraryManager.getGraphicsLibPtr();
    _gui = libraryManager.getGuiLibPtr();
    return true;
}

void GameLoop::initializeManagers() {
    // auto& textureManager = TextureManager::getInstance();
    auto& modelManager = ModelManager::getInstance();

    // textureManager.setGraphicsLib(_graphics);
    modelManager.setGraphicsLib(_graphics);
    _renderer = std::make_shared<Renderer>();
    _renderer->init(_graphics);
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
    _camera = std::make_shared<CameraController>();
    _camera->init(_graphics);
    _modelManagerAdapter = Zappy::ModelManagerAdapter::createShared();
    _modelManagerAdapter->setGraphicsLib(_graphics);
    if (_gameController)
        _mapRenderer = std::make_shared<Zappy::MapRenderer>(_graphics, _gameController->getGameState());
    _userInterface = std::make_shared<GUI::UserInterface>(_gui);
    _userInterface->initialize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    if (_coordinator) {
        _coordinator->setUINotifier(_userInterface);
        _userInterface->setCommandExecutor(_coordinator);
    }
}

int GameLoop::run() {
    if (!_graphics || !_gui || !_renderer || !_camera)
        return 84;
    while (!_graphics->WindowShouldClose()) {
        bool uiHandledMouse = _userInterface->handleMouseEvents();
        bool mouseOverUI = _userInterface->isMouseOverUI();
        int selectedTileX = -1;
        int selectedTileY = -1;
        int selectedPlayerId = -1;
        _camera->update(_graphics, uiHandledMouse, mouseOverUI);
        _graphics->BeginDrawing();
        if (_renderer)
            _renderer->renderSkybox(_graphics);
        _graphics->ClearBackground({32, 32, 64, 255});
        _graphics->UpdateMusic();
        _graphics->BeginCamera3D();
        if (_selectedTile.selected) {
            selectedTileX = _selectedTile.x;
            selectedTileY = _selectedTile.y;
        }
        if (_selectedPlayer.selected)
            selectedPlayerId = _selectedPlayer.playerId;
        if (_mapRenderer) {
            _mapRenderer->renderWithSelection(selectedTileX, selectedTileY, selectedPlayerId);
            _mapRenderer->render();
        }
        Zappy::ParticleSystem::getInstance().render(_graphics);
        Zappy::EjectionAnimationManager::getInstance().render(_graphics);
        Zappy::DeathAnimationManager::getInstance().render(_graphics);
        _graphics->EndCamera3D();
        updateGameData();
        if (_gameController)
            auto gameState = _gameController->getGameState();
        _userInterface->render();
        if (_userInterface) {
            auto timeInfoWindow = std::dynamic_pointer_cast<GUI::TimeInfoWindow>(
                _userInterface->getWindow("timeInfo"));
            if (timeInfoWindow)
                timeInfoWindow->setFPS(_graphics->GetFPS());
        }
        _graphics->EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}

void GameLoop::setServerInfo(const std::string& host, int port) {
    _host = host;
    _port = port;
}

void GameLoop::setGameController(std::shared_ptr<IGameController> controller) {
    _gameController = controller;
    if (_gameController && _graphics)
        _gameController->setGraphics(_graphics);
    if (_gameController && _graphics && _modelManagerAdapter) {
        auto gameState = _gameController->getGameState();
        if (gameState) {
            _mapRenderer = std::make_shared<Zappy::MapRenderer>(_graphics, gameState);
            _mapRenderer->initialize();
            if (gameState->isMapInitialized())
                updateCameraForMapSize();
        }
    }
}

void GameLoop::updateCameraForMapSize() {
    if (!_gameController || !_camera)
        return;
    auto gameState = _gameController->getGameState();
    if (!gameState || !gameState->isMapInitialized())
        return;
    int mapWidth = gameState->getMapWidth();
    int mapHeight = gameState->getMapHeight();
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
    _camera->reset();
    _graphics->SetMusicVolume(_gameController->getGameState()->getMusicVolume());
}

void GameLoop::setSkyboxTexture(const std::string& texturePath) {
    if (_renderer)
        _renderer->setSkyboxTexture(texturePath);
}

bool GameLoop::isSkyboxLoaded() const {
    if (!_renderer)
        return false;
    return _renderer->isSkyboxLoaded();
}

void GameLoop::setComponentCoordinator(std::shared_ptr<ComponentCoordinator> coordinator) {
    _coordinator = coordinator;
}
