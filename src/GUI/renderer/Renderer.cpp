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
}

void Renderer::render(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
    std::shared_ptr<ObjManager> modelManager, std::shared_ptr<CameraController> camera,
    std::shared_ptr<UIRenderer> uiRenderer) {

    graphics->BeginDrawing();
    renderBackground(graphics);
    graphics->BeginCamera3D();
    renderGrid(graphics);
    renderScene(graphics, modelManager);
    graphics->EndCamera3D();
    renderUI(graphics, gui, camera, uiRenderer);
    graphics->EndDrawing();
}

void Renderer::renderBackground(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->ClearBackground({32, 32, 64, 255});
}

void Renderer::renderGrid(std::shared_ptr<IGraphicsLib> graphics) {
    graphics->DrawPlane({0.0f, 0.0f, 0.0f}, {20.0f, 20.0f}, {200, 200, 200, 255});
}

void Renderer::renderScene(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<ObjManager> modelManager) {
    modelManager->renderAll(graphics);
}

void Renderer::renderUI(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<IGuiLib> gui,
                       std::shared_ptr<CameraController> camera, std::shared_ptr<UIRenderer> uiRenderer) {
    uiRenderer->renderUI(graphics, gui, camera);
}

void Renderer::renderSprite2D(int textureId, int x, int y) {
    auto& textureManager = TextureManager::getInstance();
    textureManager.drawTexture(textureId, x, y);
}

int Renderer::loadResourceTexture(const std::string& resourceName, const std::string& texturePath) {
    auto it = m_resourceTextures.find(resourceName);
    if (it != m_resourceTextures.end()) {
        return it->second;
    }
    auto& textureManager = TextureManager::getInstance();
    int textureId = textureManager.loadTexture(texturePath);
    if (textureId != -1)
        m_resourceTextures[resourceName] = textureId;
    else
        std::cerr << "Échec du chargement de la texture de ressource: " << texturePath << std::endl;
    return textureId;
}

int Renderer::getResourceTextureId(const std::string& resourceName) const {
    auto it = m_resourceTextures.find(resourceName);
    if (it != m_resourceTextures.end()) {
        return it->second;
    }
    return -1; // Texture non trouvée
}
