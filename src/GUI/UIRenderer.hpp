/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** UIRenderer
*/

#pragma once

#include <memory>
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/IGuiLib.hpp"
#include "ModelManager.hpp"
#include "CameraController.hpp"
#include "Constants.hpp"

class UIRenderer {
public:
    UIRenderer();
    ~UIRenderer() = default;

    void renderUI(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui, std::shared_ptr<CameraController> camera);
    void renderStatusBar(std::shared_ptr<IGuiLib> gui);
    void renderCameraInfo(std::shared_ptr<IGuiLib> gui, std::shared_ptr<CameraController> camera);
    void renderModelControls(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui, std::shared_ptr<ModelManager> modelManager);
    void renderFPS(std::shared_ptr<IGraphicsLib> graphics);

    bool& showUI() { return m_showUI; }
    int& fps() { return m_fps; }

private:
    bool m_showUI;
    int m_fps;
    int m_frameCounter;
    bool m_windowResized;
};
