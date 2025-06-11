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
    std::lock_guard<std::mutex> lock(m_mutex);
    m_graphicsLib = graphicsLib;
}

int TextureManager::loadTexture(const std::string& path) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_pathToId.find(path);
    if (it != m_pathToId.end()) {
        return it->second;
    }

    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé dans TextureManager" << std::endl;
        return -1;
    }
    try {
        int textureId = m_graphicsLib->LoadTexture2D(path);
        if (textureId < 0) {
            std::cerr << "Erreur lors du chargement de la texture " << path << std::endl;
            return -1;
        }
        m_pathToId[path] = textureId;
        m_idToPath[textureId] = path;
        return textureId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement de la texture " << path << ": " << e.what() << std::endl;
        return -1;
    }
}

void TextureManager::drawTexture(int textureId, int x, int y) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé" << std::endl;
        return;
    }
    if (!m_graphicsLib->IsTextureReady(textureId)) {
        std::cerr << "Erreur: Texture ID " << textureId << " non prête ou invalide" << std::endl;
        return;
    }
    m_graphicsLib->DrawTexture2D(textureId, x, y);
}

void TextureManager::unloadTexture(int textureId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_idToPath.find(textureId);
    if (it == m_idToPath.end()) {
        std::cerr << "Tentative de libération d'une texture inexistante (ID: " << textureId << ")" << std::endl;
        return;
    }
    std::string path = it->second;
    m_idToPath.erase(it);
    m_pathToId.erase(path);
    if (m_graphicsLib)
        m_graphicsLib->UnloadTexture2D(textureId);
}

void TextureManager::unloadAllTextures() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_graphicsLib) {
        for (const auto& [id, _] : m_idToPath)
            m_graphicsLib->UnloadTexture2D(id);
    }
    m_idToPath.clear();
    m_pathToId.clear();
}

bool TextureManager::hasTexture(const std::string& path) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_pathToId.find(path) != m_pathToId.end();
}

int TextureManager::getTextureId(const std::string& path) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_pathToId.find(path);
    if (it != m_pathToId.end())
        return it->second;
    return -1;
}

size_t TextureManager::getTextureCount() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_pathToId.size();
}

TextureManager::~TextureManager() {
    unloadAllTextures();
}
