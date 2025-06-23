/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** UIRenderer
*/

#include <string>
#include <memory>
#include <sstream>
#include <cmath>
#include <ctime>
#include "UIRenderer.hpp"
#include "../Constants.hpp"

UIRenderer::UIRenderer()
    : m_showUI(true),
      m_fps(60) {
}

void UIRenderer::renderUI(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui, std::shared_ptr<CameraController> camera) {
    m_fps = graphics->GetFPS();
    renderCameraInfo(gui, camera);
}

void UIRenderer::renderStatusBar(std::shared_ptr<IGuiLib> gui) {
    gui->DrawStatusBar(0, DEFAULT_HEIGHT - 25, DEFAULT_WIDTH, 25, "Mode Test - Contrôle de caméra avec la souris");
}

void UIRenderer::renderCameraInfo(std::shared_ptr<IGuiLib> gui, std::shared_ptr<CameraController> camera) {
    gui->DrawPanel(DEFAULT_WIDTH - 210, 10, 200, 80);
    gui->DrawLabel(DEFAULT_WIDTH - 200, 20, 180, 20, "Caméra:");
    std::string distanceStr = "Distance: " + std::to_string(static_cast<int>(camera->distance()));
    gui->DrawLabel(DEFAULT_WIDTH - 200, 40, 180, 20, distanceStr);
    std::string angleStr = "Angle: " + std::to_string(static_cast<int>(camera->angleY() * 180 / M_PI)) + "°";
    gui->DrawLabel(DEFAULT_WIDTH - 200, 60, 180, 20, angleStr);
}
