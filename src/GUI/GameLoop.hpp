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

namespace Zappy {
    class MapRenderer;
    class ModelManagerAdapter;
}

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
    std::string m_host;
    int m_port;
    std::shared_ptr<IGraphicsLib> m_graphics;
    std::shared_ptr<IGuiLib> m_gui;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<CameraController> m_camera;
    std::shared_ptr<GameController> m_gameController;
    std::shared_ptr<Zappy::MapRenderer> m_mapRenderer;
    std::shared_ptr<Zappy::ModelManagerAdapter> m_modelManagerAdapter;
    std::shared_ptr<GUI::UserInterface> m_userInterface;
    std::shared_ptr<NetworkManager> m_networkManager;
    int m_mapWidth = 20;
    int m_mapHeight = 20;
    float m_gameTime = 0.0f;
    int m_frequency = 100;
    int m_gameTick = 0;
    struct {
        int x = 0;
        int y = 0;
        bool selected = false;
    } m_selectedTile;
    struct {
        int playerId = -1;
        bool selected = false;
    } m_selectedPlayer;
};
