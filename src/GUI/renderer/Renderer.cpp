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
#include "../Constants.hpp"

Renderer::Renderer()
    : m_mapWidth(20),
      m_mapHeight(20) {
}

void Renderer::init(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_TITLE);
    m_graphicsLib = graphics;
}

void Renderer::render(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
    std::shared_ptr<CameraController> camera,
    std::shared_ptr<UIRenderer> uiRenderer) {

    graphics->BeginDrawing();
    renderBackground(graphics);
    graphics->BeginCamera3D();
    renderGrid(graphics);
    renderScene(graphics);
    graphics->EndCamera3D();
    renderUI(graphics, gui, camera, uiRenderer);
    graphics->EndDrawing();
}

void Renderer::renderBackground(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->ClearBackground({32, 32, 64, 255});
}

void Renderer::renderGrid(std::shared_ptr<IGraphicsLib> graphics) {
    // Position le plan pour qu'il soit aligné avec le coin inférieur gauche à l'origine (0,0,0)
    // La grille s'étend donc de (0,0) à (20,0,20) au lieu d'être centrée sur l'origine
    graphics->DrawPlane({10.0f, 0.0f, 10.0f}, {20.0f, 20.0f}, {200, 200, 200, 255});
}

void Renderer::renderScene(std::shared_ptr<IGraphicsLib> graphics) {
    auto& modelManager = ModelManager::getInstance();
    (void)modelManager;
    (void)graphics;
    //TODO(Sam): Implement scene rendering logic
}

void Renderer::renderUI(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
                       std::shared_ptr<CameraController> camera, std::shared_ptr<UIRenderer> uiRenderer) {
    uiRenderer->renderUI(graphics, gui, camera);
}

void Renderer::renderSprite2D(int textureId, int x, int y) {
    if (auto graphics = m_graphicsLib.lock()) {
        graphics->DrawTexture2D(textureId, x, y);
    }
}

void Renderer::renderModelFromManager(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color) {
    if (auto graphics = m_graphicsLib.lock())
        graphics->DrawModel3D(modelId, position, 1.0f, color);
}

int Renderer::loadResourceTexture(const std::string& resourceName, const std::string& texturePath) {
    auto it = m_resourceTextures.find(resourceName);
    if (it != m_resourceTextures.end()) {
        return it->second;
    }
    if (auto graphics = m_graphicsLib.lock()) {
        int textureId = graphics->LoadTexture2D(texturePath);
        if (textureId != -1)
            m_resourceTextures[resourceName] = textureId;
        else
            std::cerr << "Échec du chargement de la texture de ressource: " << texturePath << std::endl;
        return textureId;
    }
    return -1;
}

int Renderer::getResourceTextureId(const std::string& resourceName) const {
    auto it = m_resourceTextures.find(resourceName);
    if (it != m_resourceTextures.end()) {
        return it->second;
    }
    return -1;
}
