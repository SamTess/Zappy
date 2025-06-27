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
    if (_skybox)
        _skybox->init(graphics);
}

void Renderer::render(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->BeginDrawing();
    renderSkybox(graphics);
    graphics->BeginCamera3D();
    Zappy::ParticleSystem::getInstance().render(graphics);
    Zappy::EjectionAnimationManager::getInstance().render(graphics);
    graphics->EndCamera3D();
    graphics->EndDrawing();
}

void Renderer::renderSkybox(std::shared_ptr<IGraphicsLib> graphics) {
    if (_skybox)
        _skybox->render(graphics);
}

void Renderer::setSkyboxTexture(const std::string& texturePath) {
    if (_skybox) {
        if (auto graphics = _graphicsLib.lock())
            _skybox->setSkyboxTexture(texturePath, graphics);
    }
}

bool Renderer::isSkyboxLoaded() const {
    return _skybox && _skybox->isLoaded();
}
