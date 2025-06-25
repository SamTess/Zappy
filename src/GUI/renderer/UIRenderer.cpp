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
    (void)camera;
    (void)gui;
}
