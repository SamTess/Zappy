/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Renderer
*/

#pragma once

#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/IGuiLib.hpp"
#include "ModelManager.hpp"
#include "CameraController.hpp"
#include "UIRenderer.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void init(IGraphicsLib& graphics);
    void render(IGraphicsLib& graphics, IGuiLib& gui, ModelManager& modelManager, CameraController& camera, UIRenderer& uiRenderer);
    
    void renderBackground(IGraphicsLib& graphics);
    void renderGrid(IGraphicsLib& graphics);
    void renderScene(IGraphicsLib& graphics, ModelManager& modelManager);
    void renderUI(IGraphicsLib& graphics, IGuiLib& gui, CameraController& camera, UIRenderer& uiRenderer);

    int getMapWidth() const { return m_mapWidth; }
    int getMapHeight() const { return m_mapHeight; }

private:
    int m_mapWidth;
    int m_mapHeight;
};
