/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Renderer
*/

#include <memory>
#include <map>
#include <string>
#include <iostream>
#include "Renderer.hpp"
#include "Skybox.hpp"
#include "ParticleSystem.hpp"
#include "EjectionAnimationManager.hpp"
#include "../Constants.hpp"

Renderer::Renderer()
    : _mapWidth(20), _mapHeight(20), _skybox(std::make_unique<Skybox>()) {
}

void Renderer::init(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_TITLE);
    _graphicsLib = graphics;
    if (_skybox) {
        _skybox->init(graphics);
    }
}

void Renderer::render(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->BeginDrawing();
    renderSkybox(graphics);
    renderBackground(graphics);
    graphics->BeginCamera3D();
    renderGrid(graphics);
    renderScene(graphics);
    graphics->EndCamera3D();
    graphics->EndDrawing();
}

void Renderer::renderBackground(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->ClearBackground({32, 32, 64, 255});
}

void Renderer::renderGrid(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->DrawPlane({10.0f, 0.0f, 10.0f}, {20.0f, 20.0f}, {200, 200, 200, 255});
}

void Renderer::renderScene(std::shared_ptr<IGraphicsLib> graphics) {
    auto& modelManager = ModelManager::getInstance();
    (void)modelManager;

    Zappy::ParticleSystem::getInstance().render(graphics);
    Zappy::EjectionAnimationManager::getInstance().render(graphics);
}

void Renderer::renderSprite2D(int textureId, int x, int y) {
    if (auto graphics = _graphicsLib.lock()) {
        graphics->DrawTexture2D(textureId, x, y);
    }
}

void Renderer::renderModelFromManager(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color) {
    if (auto graphics = _graphicsLib.lock())
        graphics->DrawModel3D(modelId, position, 1.0f, color);
}

int Renderer::loadResourceTexture(const std::string& resourceName, const std::string& texturePath) {
    auto it = _resourceTextures.find(resourceName);
    if (it != _resourceTextures.end()) {
        return it->second;
    }
    if (auto graphics = _graphicsLib.lock()) {
        int textureId = graphics->LoadTexture2D(texturePath);
        if (textureId != -1)
            _resourceTextures[resourceName] = textureId;
        else
            std::cerr << "Échec du chargement de la texture de ressource: " << texturePath << std::endl;
        return textureId;
    }
    return -1;
}

int Renderer::getResourceTextureId(const std::string& resourceName) const {
    auto it = _resourceTextures.find(resourceName);
    if (it != _resourceTextures.end()) {
        return it->second;
    }
    return -1;
}

void Renderer::renderSkybox(std::shared_ptr<IGraphicsLib> graphics) {
    if (_skybox) {
        _skybox->render(graphics);
    }
}

void Renderer::setSkyboxTexture(const std::string& texturePath) {
    if (_skybox) {
        if (auto graphics = _graphicsLib.lock()) {
            _skybox->setSkyboxTexture(texturePath, graphics);
        }
    }
}

bool Renderer::isSkyboxLoaded() const {
    return _skybox && _skybox->isLoaded();
}
