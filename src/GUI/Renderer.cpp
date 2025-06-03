/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Renderer
*/

#include "Renderer.hpp"

Renderer::Renderer()
    : m_mapWidth(20),
      m_mapHeight(20)
{
}

void Renderer::init(IGraphicsLib& graphics)
{
    graphics.InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_TITLE);
}

void Renderer::render(IGraphicsLib& graphics, IGuiLib& gui, ModelManager& modelManager, CameraController& camera, UIRenderer& uiRenderer)
{
    graphics.BeginDrawing();
    
    renderBackground(graphics);
    
    graphics.BeginCamera3D();
    renderGrid(graphics);
    renderScene(graphics, modelManager);
    graphics.EndCamera3D();
    
    renderUI(graphics, gui, camera, uiRenderer);
    
    graphics.EndDrawing();
}

void Renderer::renderBackground(IGraphicsLib& graphics)
{
    graphics.ClearBackground({32, 32, 64, 255});
}

void Renderer::renderGrid(IGraphicsLib& graphics)
{
    graphics.DrawPlane({0.0f, 0.0f, 0.0f}, {20.0f, 20.0f}, {200, 200, 200, 255});
}

void Renderer::renderScene(IGraphicsLib& graphics, ModelManager& modelManager)
{
    modelManager.drawModel(graphics);
}

void Renderer::renderUI(IGraphicsLib& graphics, IGuiLib& gui, CameraController& camera, UIRenderer& uiRenderer)
{
    uiRenderer.renderUI(graphics, gui, camera);
}
