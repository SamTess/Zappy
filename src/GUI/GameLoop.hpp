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

class GameLoop {
public:
    GameLoop();
    ~GameLoop() = default;
    bool init();
    int run();
    void setServerInfo(const std::string& host, int port);
    void renderCube();

private:
    bool loadLibraries();
    void initializeManagers();
    bool loadModels();
    void setupComponents();

    std::string m_host;
    int m_port;

    std::shared_ptr<IGraphicsLib> m_graphics;
    std::shared_ptr<IGuiLib> m_gui;
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<CameraController> m_camera;
    std::shared_ptr<UIRenderer> m_uiRenderer;

    int m_cubeModelId = -1;
};
