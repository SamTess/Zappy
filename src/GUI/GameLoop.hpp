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
#include "renderer/MapRendererObserver.hpp"
#include "textureManager/TextureManager.hpp"
#include "graphicalContext/GraphicalContext.hpp"

namespace Zappy {
    class MapRenderer;
    class ModelManagerAdapter;
}

class GameLoop : public std::enable_shared_from_this<GameLoop> {
public:
    GameLoop();
    ~GameLoop() = default;
    bool init();
    int run();
    void setServerInfo(const std::string& host, int port);
    void setGraphicalContext(std::shared_ptr<GraphicalContext> context);

private:
    bool loadLibraries();
    void initializeManagers();
    void setupComponents();
    bool loadModels();

    std::string m_host;
    int m_port;

    int m_mapWidth = 20;
    int m_mapHeight = 20;

    std::shared_ptr<IGraphicsLib> m_graphics;
    std::shared_ptr<IGuiLib> m_gui;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<CameraController> m_camera;
    std::shared_ptr<UIRenderer> m_uiRenderer;
    std::shared_ptr<GraphicalContext> m_context;
    std::shared_ptr<Zappy::MapRenderer> m_mapRenderer;
    std::shared_ptr<Zappy::MapRendererObserver> m_mapRendererObserver;
    std::shared_ptr<Zappy::ModelManagerAdapter> m_modelManagerAdapter;

    int m_cubeModelId = -1;
};
