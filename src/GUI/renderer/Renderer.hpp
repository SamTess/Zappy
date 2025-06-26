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
#include "../textureManager/TextureManager.hpp"
#include "../textureManager/ModelManager.hpp"
#include "Skybox.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void init(std::shared_ptr<IGraphicsLib> graphics);
    void render(std::shared_ptr<IGraphicsLib> graphics);
    void renderSkybox(std::shared_ptr<IGraphicsLib> graphics);
    int getMapWidth() const { return _mapWidth; }
    int getMapHeight() const { return _mapHeight; }
    void setSkyboxTexture(const std::string& texturePath);
    bool isSkyboxLoaded() const;

private:
    int _mapWidth;
    int _mapHeight;
    std::map<std::string, int> _resourceTextures;
    std::weak_ptr<IGraphicsLib> _graphicsLib;
    std::unique_ptr<Skybox> _skybox;
};
