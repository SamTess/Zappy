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
    bool isLoaded() const { return m_textureId != -1; }
    bool loadDefaultSkybox(std::shared_ptr<IGraphicsLib> graphics);

private:
    void renderPNGSkybox(std::shared_ptr<IGraphicsLib> graphics);

    int m_textureId = -1;
    std::string m_texturePath = "assets/models/sky.jpg";
    bool m_initialized = false;
    int m_screenWidth = 1920;
    int m_screenHeight = 1080;
};
