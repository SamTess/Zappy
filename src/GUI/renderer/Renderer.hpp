/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Renderer
*/

#pragma once

#include <memory>
#include <map>
#include <string>
#include "../../Shared/IGraphicsLib.hpp"
#include "../../Shared/IGuiLib.hpp"
#include "../cameraController/CameraController.hpp"
#include "UIRenderer.hpp"
#include "../obj/ObjManager.hpp"
#include "../textureManager/TextureManager.hpp"
#include "../textureManager/ModelManager.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void init(std::shared_ptr<IGraphicsLib> graphics);
    void render(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
                std::shared_ptr<ObjManager> modelManager, std::shared_ptr<CameraController> camera,
                std::shared_ptr<UIRenderer> uiRenderer);

    void renderBackground(std::shared_ptr<IGraphicsLib> graphics);
    void renderGrid(std::shared_ptr<IGraphicsLib> graphics);
    void renderScene(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<ObjManager> modelManager);
    void renderUI(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
                 std::shared_ptr<CameraController> camera, std::shared_ptr<UIRenderer> uiRenderer);
    void renderSprite2D(int textureId, int x, int y);
    void renderModelFromManager(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color = {255, 255, 255, 255});


    int loadResourceTexture(const std::string& resourceName, const std::string& texturePath);
    int getResourceTextureId(const std::string& resourceName) const;

    int getMapWidth() const { return m_mapWidth; }
    int getMapHeight() const { return m_mapHeight; }

private:
    int m_mapWidth;
    int m_mapHeight;
    std::map<std::string, int> m_resourceTextures;
    std::weak_ptr<IGraphicsLib> m_graphicsLib;
};
