/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TextureManager - Implementation
*/

#include "TextureManager.hpp"
#include <iostream>
#include <string>

void TextureManager::setGraphicsLib(std::shared_ptr<IGraphicsLib> graphicsLib) {
    std::lock_guard<std::mutex> lock(_mutex);
    _graphicsLib = graphicsLib;
}

int TextureManager::loadTexture(const std::string& path) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _pathToId.find(path);
    if (it != _pathToId.end()) {
        return it->second;
    }

    if (!_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé dans TextureManager" << std::endl;
        return -1;
    }
    try {
        int textureId = _graphicsLib->LoadTexture2D(path);
        if (textureId < 0) {
            std::cerr << "Erreur lors du chargement de la texture " << path << std::endl;
            return -1;
        }
        _pathToId[path] = textureId;
        _idToPath[textureId] = path;
        return textureId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement de la texture " << path << ": " << e.what() << std::endl;
        return -1;
    }
}

void TextureManager::drawTexture(int textureId, int x, int y) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé" << std::endl;
        return;
    }
    if (!_graphicsLib->IsTextureReady(textureId)) {
        std::cerr << "Erreur: Texture ID " << textureId << " non prête ou invalide" << std::endl;
        return;
    }
    _graphicsLib->DrawTexture2D(textureId, x, y);
}

void TextureManager::unloadTexture(int textureId) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _idToPath.find(textureId);
    if (it == _idToPath.end()) {
        std::cerr << "Tentative de libération d'une texture inexistante (ID: " << textureId << ")" << std::endl;
        return;
    }
    std::string path = it->second;
    _idToPath.erase(it);
    _pathToId.erase(path);
    if (_graphicsLib)
        _graphicsLib->UnloadTexture2D(textureId);
}

void TextureManager::unloadAllTextures() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_graphicsLib) {
        for (const auto& [id, _] : _idToPath)
            _graphicsLib->UnloadTexture2D(id);
    }
    _idToPath.clear();
    _pathToId.clear();
}

bool TextureManager::hasTexture(const std::string& path) const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _pathToId.find(path) != _pathToId.end();
}

int TextureManager::getTextureId(const std::string& path) const {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _pathToId.find(path);
    if (it != _pathToId.end())
        return it->second;
    return -1;
}

size_t TextureManager::getTextureCount() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _pathToId.size();
}

TextureManager::~TextureManager() {
    unloadAllTextures();
}
