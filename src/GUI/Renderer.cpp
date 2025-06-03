/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Renderer
*/

#include <memory>
#include "Renderer.hpp"

Renderer::Renderer()
    : m_mapWidth(20),
      m_mapHeight(20) {
}

void Renderer::init(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_TITLE);
}

void Renderer::render(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
    std::shared_ptr<ModelManager> modelManager, std::shared_ptr<CameraController> camera,
    std::shared_ptr<UIRenderer> uiRenderer) {

    graphics->BeginDrawing();
    renderBackground(graphics);
    graphics->BeginCamera3D();
    renderGrid(graphics);
    renderScene(graphics, modelManager);
    graphics->EndCamera3D();
    renderUI(graphics, gui, camera, uiRenderer);
    graphics->EndDrawing();
}

void Renderer::renderBackground(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->ClearBackground({32, 32, 64, 255});
}

void Renderer::renderGrid(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->DrawPlane({0.0f, 0.0f, 0.0f}, {20.0f, 20.0f}, {200, 200, 200, 255});
}

void Renderer::renderScene(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<ModelManager> modelManager) {
    modelManager->renderAll(graphics);
}

void Renderer::renderUI(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
                       std::shared_ptr<CameraController> camera, std::shared_ptr<UIRenderer> uiRenderer) {
    uiRenderer->renderUI(graphics, gui, camera);
}
