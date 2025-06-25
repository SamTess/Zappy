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
    _textureId = graphics->LoadTexture2D(_texturePath);
    if (_textureId == -1) {
        std::cerr << "Failed to load skybox texture: " << _texturePath << std::endl;
        _initialized = false;
        return false;
    }
    _initialized = true;
    return true;
}

void Skybox::render(std::shared_ptr<IGraphicsLib> graphics) {
    if (!_initialized || !graphics) {
        return;
    }

    renderPNGSkybox(graphics);
}

void Skybox::renderPNGSkybox(std::shared_ptr<IGraphicsLib> graphics) {
    if (_textureId == -1) {
        ZappyTypes::Color skyColor = {135, 206, 235, 255};
        graphics->ClearBackground(skyColor);
        return;
    }
    graphics->DrawTexture2D(_textureId, 0, 0);
}

void Skybox::cleanup(std::shared_ptr<IGraphicsLib> graphics) {
    if (!graphics) {
        return;
    }
    if (_textureId != -1) {
        graphics->UnloadTexture2D(_textureId);
        _textureId = -1;
    }
    _initialized = false;
}

void Skybox::setSkyboxTexture(const std::string& texturePath, std::shared_ptr<IGraphicsLib> graphics) {
    if (!graphics) {
        std::cerr << "Graphics library is null" << std::endl;
        return;
    }
    if (_textureId != -1) {
        graphics->UnloadTexture2D(_textureId);
        _textureId = -1;
    }
    _textureId = graphics->LoadTexture2D(texturePath);
    if (_textureId != -1) {
        _texturePath = texturePath;
        std::cout << "Skybox texture loaded: " << texturePath << std::endl;
    } else {
        std::cerr << "Failed to load skybox texture: " << texturePath << std::endl;
    }
}
