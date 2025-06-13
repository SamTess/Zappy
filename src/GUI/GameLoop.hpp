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
#include "renderer/UIRenderer.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/MapRenderer.hpp"
#include "textureManager/TextureManager.hpp"
#include "gameController/GameController.hpp"
#include "./network/networkManager/NetworkManager.hpp"
#include "graphicalContext/GraphicalContext.hpp"
#include "ui/UserInterface.hpp"
#include "shared/GameData.hpp"

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

    std::string m_host;
    int m_port;

    int m_mapWidth = 20;
    int m_mapHeight = 20;

    std::shared_ptr<IGraphicsLib> m_graphics;
    std::shared_ptr<IGuiLib> m_gui;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<CameraController> m_camera;
    std::shared_ptr<UIRenderer> m_uiRenderer;
    std::shared_ptr<GameController> m_gameController;
    std::shared_ptr<Zappy::MapRenderer> m_mapRenderer;
    std::shared_ptr<Zappy::ModelManagerAdapter> m_modelManagerAdapter;
    std::shared_ptr<NetworkManager> m_networkManager;
    std::shared_ptr<GUI::UserInterface> m_userInterface;
    GUI::GameData m_gameData;

    int m_cubeModelId = -1;
    struct {
        int x = 0;
        int y = 0;
        bool selected = false;
    } m_selectedTile;
    void updateGameData();
    void handleTileSelection(int x, int y);
    void handleViewModeChange(int mode);
};
