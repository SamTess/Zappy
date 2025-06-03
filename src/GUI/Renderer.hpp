/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Renderer
*/

#pragma once

#include <memory>
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/IGuiLib.hpp"
#include "ModelManager.hpp"
#include "CameraController.hpp"
#include "UIRenderer.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void init(std::shared_ptr<IGraphicsLib> graphics);
    void render(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
                std::shared_ptr<ModelManager> modelManager, std::shared_ptr<CameraController> camera,
                std::shared_ptr<UIRenderer> uiRenderer);

    void renderBackground(std::shared_ptr<IGraphicsLib> graphics);
    void renderGrid(std::shared_ptr<IGraphicsLib> graphics);
    void renderScene(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<ModelManager> modelManager);
    void renderUI(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
                 std::shared_ptr<CameraController> camera, std::shared_ptr<UIRenderer> uiRenderer);

    int getMapWidth() const { return m_mapWidth; }
    int getMapHeight() const { return m_mapHeight; }

private:
    int m_mapWidth;
    int m_mapHeight;
};
