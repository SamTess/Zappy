/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameLoop
*/

#pragma once

#include <memory>
#include <string>
#include "../Shared/LibraryManager.hpp"
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/IGuiLib.hpp"
#include "cameraController/CameraController.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/MapRenderer.hpp"
#include "textureManager/TextureManager.hpp"
#include "gameController/GameController.hpp"
#include "gameController/GameState.hpp"
#include "ui/UserInterface.hpp"
#include "./network/networkManager/NetworkManager.hpp"
#include "shared/services/ComponentCoordinator.hpp"

namespace Zappy {
    class MapRenderer;
    class ModelManagerAdapter;
}

typedef struct _selectedTile_t {
    int x;
    int y;
    bool selected;
} SelectedTile_s;

typedef struct _selectedPlayer_t {
    int playerId;
    bool selected;
} SelectedPlayer_s;

class GameLoop {
public:
    explicit GameLoop(std::shared_ptr<NetworkManager> networkManager);
    ~GameLoop() = default;
    bool init();
    int run();
    void setServerInfo(const std::string& host, int port);
    void setGameController(std::shared_ptr<GameController> controller);
    void setSkyboxTexture(const std::string& texturePath);
    bool isSkyboxLoaded() const;
    void setComponentCoordinator(std::shared_ptr<ComponentCoordinator> coordinator);

private:
    bool loadLibraries();
    void initializeManagers();
    void setupComponents();
    bool loadModels();
    void updateCameraForMapSize();
    void updateGameData();
    void handleTileSelection(int x, int y);
    void handleViewModeChange(int mode);
    void onMapSizeChanged(int width, int height);
    void onTileChanged(int x, int y, const std::shared_ptr<const ITile>& tile);
    bool worldToTileCoordinates(ZappyTypes::Vector3 worldPos, std::shared_ptr<int> tileX, std::shared_ptr<int> tileY);
    bool performTileSelection(ZappyTypes::Vector2 screenPos, std::shared_ptr<int> tileX, std::shared_ptr<int> tileY);
    bool performPlayerSelection(ZappyTypes::Vector2 screenPos, std::shared_ptr<int> playerId);
    void handlePlayerSelection(int playerId);
    float calculateRayToPointDistance(ZappyTypes::Vector3 rayOrigin, ZappyTypes::Vector3 rayDirection, ZappyTypes::Vector3 point);
    ZappyTypes::Vector3 calculatePlayerWorldPosition(int playerX, int playerY, int playerIndex, int totalPlayers);
    std::string _host;
    int _port;
    std::shared_ptr<IGraphicsLib> _graphics;
    std::shared_ptr<IGuiLib> _gui;
    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<CameraController> _camera;
    std::shared_ptr<GameController> _gameController;
    std::shared_ptr<Zappy::MapRenderer> _mapRenderer;
    std::shared_ptr<Zappy::ModelManagerAdapter> _modelManagerAdapter;
    std::shared_ptr<GUI::UserInterface> _userInterface;
    std::shared_ptr<NetworkManager> _networkManager;
    std::shared_ptr<ComponentCoordinator> _coordinator;
    int _mapWidth = 20;
    int _mapHeight = 20;
    float _gameTime = 0.0f;
    int _frequency = 100;
    int _gameTick = 0;
    SelectedTile_s _selectedTile;
    SelectedPlayer_s _selectedPlayer;
};
