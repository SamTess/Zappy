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
#include "textureManager/TextureManager.hpp"
#include "graphicalContext/GraphicalContext.hpp"

class GameLoop : public IGraphicalContextObserver {
public:
    GameLoop();
    ~GameLoop() = default;
    bool init();
    int run();
    void setServerInfo(const std::string& host, int port);
    void renderCube();
    
    // Implementation of IGraphicalContextObserver
    void onMapSizeChanged(int width, int height) override;
    void onTileChanged(int x, int y, const TileData& tileData) override;

private:
    bool loadLibraries();
    void initializeManagers();
    void setupComponents();
    bool loadModels();

    std::string m_host;
    int m_port;
    
    // Map dimensions from GraphicalContext
    int m_mapWidth = 20;
    int m_mapHeight = 20;

    std::shared_ptr<IGraphicsLib> m_graphics;
    std::shared_ptr<IGuiLib> m_gui;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<CameraController> m_camera;
    std::shared_ptr<UIRenderer> m_uiRenderer;

    int m_cubeModelId = -1;
};
