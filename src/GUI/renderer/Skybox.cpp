/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Skybox implementation
*/

#include "Skybox.hpp"
#include <iostream>
#include <memory>
#include <string>

Skybox::Skybox() {
}

bool Skybox::init(std::shared_ptr<IGraphicsLib> graphics) {
    if (!graphics) {
        std::cerr << "Graphics library is null" << std::endl;
        return false;
    }
    return loadDefaultSkybox(graphics);
}

bool Skybox::loadDefaultSkybox(std::shared_ptr<IGraphicsLib> graphics) {
    if (!graphics) {
        std::cerr << "Graphics library is null" << std::endl;
        return false;
    }
    m_textureId = graphics->LoadTexture2D(m_texturePath);
    if (m_textureId == -1) {
        std::cerr << "Failed to load skybox texture: " << m_texturePath << std::endl;
        m_initialized = false;
        return false;
    }
    m_initialized = true;
    return true;
}

void Skybox::render(std::shared_ptr<IGraphicsLib> graphics) {
    if (!m_initialized || !graphics) {
        return;
    }

    renderPNGSkybox(graphics);
}

void Skybox::renderPNGSkybox(std::shared_ptr<IGraphicsLib> graphics) {
    if (m_textureId == -1) {
        ZappyTypes::Color skyColor = {135, 206, 235, 255};
        graphics->ClearBackground(skyColor);
        return;
    }
    graphics->DrawTexture2D(m_textureId, 0, 0);
}

void Skybox::cleanup(std::shared_ptr<IGraphicsLib> graphics) {
    if (!graphics) {
        return;
    }
    if (m_textureId != -1) {
        graphics->UnloadTexture2D(m_textureId);
        m_textureId = -1;
    }
    m_initialized = false;
}

void Skybox::setSkyboxTexture(const std::string& texturePath, std::shared_ptr<IGraphicsLib> graphics) {
    if (!graphics) {
        std::cerr << "Graphics library is null" << std::endl;
        return;
    }
    if (m_textureId != -1) {
        graphics->UnloadTexture2D(m_textureId);
        m_textureId = -1;
    }
    m_textureId = graphics->LoadTexture2D(texturePath);
    if (m_textureId != -1) {
        m_texturePath = texturePath;
        std::cout << "Skybox texture loaded: " << texturePath << std::endl;
    } else {
        std::cerr << "Failed to load skybox texture: " << texturePath << std::endl;
    }
}
