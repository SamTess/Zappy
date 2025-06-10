/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager - Implementation
*/

#include "ModelManager.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <utility>

void ModelManager::setGraphicsLib(std::shared_ptr<IGraphicsLib> graphicsLib) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_graphicsLib = graphicsLib;
}

int ModelManager::loadModel(const std::string& modelPath, const std::string& texturePath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_pathToId.find(modelPath);
    if (it != m_pathToId.end()) {
        return it->second;
    }
    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé dans ModelManager" << std::endl;
        return -1;
    }

    try {
        m_graphicsLib->LoadModel3D(modelPath);
        int modelId = m_nextModelId++;
        Model3D model;
        model.modelId = modelId;
        model.scale = {1.0f, 1.0f, 1.0f};
        model.boundingBoxMin = {0.0f, 0.0f, 0.0f};
        model.boundingBoxMax = {0.0f, 0.0f, 0.0f};
        if (!texturePath.empty()) {
            int textureId = m_graphicsLib->LoadTexture2D(texturePath);
            if (textureId >= 0) {
                model.textureIds.push_back(textureId);
            } else {
                std::cerr << "Avertissement: Échec du chargement de la texture " << texturePath << std::endl;
            }
        }
        m_pathToId[modelPath] = modelId;
        m_models[modelId] = model;
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle " << modelPath << ": " << e.what() << std::endl;
        return -1;
    }
}

int ModelManager::loadModelWithTextures(const std::string& modelPath, const std::vector<std::string>& texturePaths) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_pathToId.find(modelPath);
    if (it != m_pathToId.end())
        return it->second;
    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé dans ModelManager" << std::endl;
        return -1;
    }
    try {
        m_graphicsLib->LoadModel3D(modelPath);
        int modelId = m_nextModelId++;
        Model3D model;
        model.modelId = modelId;
        model.scale = {1.0f, 1.0f, 1.0f};
        model.boundingBoxMin = {0.0f, 0.0f, 0.0f};
        model.boundingBoxMax = {0.0f, 0.0f, 0.0f};
        for (const auto& texturePath : texturePaths) {
            int textureId = m_graphicsLib->LoadTexture2D(texturePath);
            if (textureId >= 0) {
                model.textureIds.push_back(textureId);
            } else {
                std::cerr << "Avertissement: Échec du chargement de la texture " << texturePath << std::endl;
            }
        }
        m_pathToId[modelPath] = modelId;
        m_models[modelId] = model;
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle " << modelPath << ": " << e.what() << std::endl;
        return -1;
    }
}

void ModelManager::drawModel(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé" << std::endl;
        return;
    }
    auto it = m_models.find(modelId);
    if (it == m_models.end()) {
        std::cerr << "Erreur: Modèle ID " << modelId << " non trouvé" << std::endl;
        return;
    }
    m_graphicsLib->DrawModel3D(position, 1.0f, color);
}

void ModelManager::drawModelEx(int modelId, ZappyTypes::Vector3 position,
    ZappyTypes::Vector3 rotationAxis,
    float rotationAngle, float scale) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé" << std::endl;
        return;
    }
    auto it = m_models.find(modelId);
    if (it == m_models.end()) {
        std::cerr << "Erreur: Modèle ID " << modelId << " non trouvé" << std::endl;
        return;
    }
    m_graphicsLib->DrawModelEx(position, rotationAxis, rotationAngle, scale);
}

void ModelManager::unloadModel(int modelId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto modelIt = m_models.find(modelId);

    if (modelIt == m_models.end()) {
        std::cerr << "Tentative de libération d'un modèle inexistant (ID: " << modelId << ")" << std::endl;
        return;
    }
    if (m_graphicsLib) {
        for (int textureId : modelIt->second.textureIds)
            m_graphicsLib->UnloadTexture2D(textureId);
        m_graphicsLib->UnloadModel3D();
    }
    for (auto it = m_pathToId.begin(); it != m_pathToId.end(); ) {
        if (it->second == modelId) {
            it = m_pathToId.erase(it);
        } else {
            ++it;
        }
    }
    m_models.erase(modelIt);
}

void ModelManager::unloadAllModels() {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_graphicsLib) {
        for (const auto& [modelId, model] : m_models) {
            for (int textureId : model.textureIds) {
                m_graphicsLib->UnloadTexture2D(textureId);
            }
            m_graphicsLib->UnloadModel3D();
        }
    }

    m_models.clear();
    m_pathToId.clear();
}

bool ModelManager::hasModel(const std::string& modelPath) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_pathToId.find(modelPath) != m_pathToId.end();
}

int ModelManager::getModelId(const std::string& modelPath) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_pathToId.find(modelPath);
    if (it != m_pathToId.end()) {
        return it->second;
    }
    return -1;
}

size_t ModelManager::getModelCount() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_models.size();
}

std::pair<ZappyTypes::Vector3, ZappyTypes::Vector3> ModelManager::getBoundingBox(int modelId) const {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_models.find(modelId);
    if (it != m_models.end()) {
        return {it->second.boundingBoxMin, it->second.boundingBoxMax};
    }
    return {ZappyTypes::Vector3{0, 0, 0}, ZappyTypes::Vector3{0, 0, 0}};
}

ModelManager::~ModelManager() {
    unloadAllModels();
}
