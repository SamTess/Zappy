/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Skybox
*/

#pragma once

#include <memory>
#include <string>
#include "../../Shared/IGraphicsLib.hpp"

class Skybox {
public:
    Skybox();
    ~Skybox() = default;

    bool init(std::shared_ptr<IGraphicsLib> graphics);
    void render(std::shared_ptr<IGraphicsLib> graphics);
    void cleanup(std::shared_ptr<IGraphicsLib> graphics);

    void setSkyboxTexture(const std::string& texturePath, std::shared_ptr<IGraphicsLib> graphics);
    bool isLoaded() const { return _textureId != -1; }
    bool loadDefaultSkybox(std::shared_ptr<IGraphicsLib> graphics);

private:
    void renderPNGSkybox(std::shared_ptr<IGraphicsLib> graphics);

    int _textureId = -1;
    std::string _texturePath = "assets/models/sky.jpg";
    bool _initialized = false;
    int _screenWidth = 1920;
    int _screenHeight = 1080;
};
