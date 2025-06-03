/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** UIRenderer
*/

#pragma once

#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/IGuiLib.hpp"
#include "ModelManager.hpp"
#include "CameraController.hpp"
#include "Constants.hpp"

class UIRenderer {
public:
    UIRenderer();
    ~UIRenderer() = default;

    void renderUI(IGraphicsLib& graphics, IGuiLib& gui, CameraController& camera);
    void renderStatusBar(IGuiLib& gui);
    void renderCameraInfo(IGuiLib& gui, CameraController& camera);
    void renderModelControls(IGraphicsLib& graphics, IGuiLib& gui, ModelManager& modelManager);
    void renderFPS(IGraphicsLib& graphics);

    bool& showUI() { return m_showUI; }
    int& fps() { return m_fps; }

private:
    bool m_showUI;
    int m_fps;
    int m_frameCounter;
    bool m_windowResized;
};
