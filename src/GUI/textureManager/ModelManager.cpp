/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager - Implementation
*/

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "ModelManager.hpp"
#include "TextureManager.hpp"

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
        int modelId;
        if (!texturePath.empty())
            modelId = m_graphicsLib->LoadModelWithTexture(modelPath, texturePath);
        else
            modelId = m_graphicsLib->LoadModel3D(modelPath);
        if (modelId < 0) {
            std::cerr << "Erreur: Échec du chargement du modèle " << modelPath << std::endl;
            return -1;
        }
        Model3D model = createModelObject(modelId);
        if (!texturePath.empty()) {
            loadTextureForModel(model, texturePath);
        }
        registerModel(modelPath, modelId, model);
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle " << modelPath << ": " << e.what() << std::endl;
        return -1;
    }
}

int ModelManager::loadModelWithTextures(const std::string& modelPath, const std::vector<std::string>& texturePaths) {
    std::lock_guard<std::mutex> lock(m_mutex);
    int cachedModelId = checkModelCache(modelPath);
    if (cachedModelId != -1)
        return cachedModelId;
    if (!checkGraphicsLibInitialized())
        return -1;
    try {
        int modelId;
        if (!texturePaths.empty())
            modelId = m_graphicsLib->LoadModelWithTexture(modelPath, texturePaths[0]);
        else
            modelId = m_graphicsLib->LoadModel3D(modelPath);
        if (modelId < 0) {
            std::cerr << "Erreur: Échec du chargement du modèle " << modelPath << std::endl;
            return -1;
        }
        Model3D model = createModelObject(modelId);
        loadTexturesForModel(model, texturePaths);
        registerModel(modelPath, modelId, model);
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle " << modelPath << ": " << e.what() << std::endl;
        return -1;
    }
}

void ModelManager::drawModel(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!validateModelForDrawing(modelId))
        return;
    m_graphicsLib->DrawModel3D(modelId, position, 1.0f, color);
}

void ModelManager::drawModelEx(int modelId, ZappyTypes::Vector3 position,
    ZappyTypes::Vector3 rotationAxis,
    float rotationAngle, float scale) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!validateModelForDrawing(modelId))
        return;
    m_graphicsLib->DrawModelEx(modelId, position, rotationAxis, rotationAngle, scale);
}

void ModelManager::unloadModel(int modelId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto modelIt = m_models.find(modelId);

    if (modelIt == m_models.end()) {
        std::cerr << "Tentative de libération d'un modèle inexistant (ID: " << modelId << ")" << std::endl;
        return;
    }
    unloadModelTextures(modelIt->second);
    unloadModelFromGraphicsLib(modelId);
    removeModelPathReferences(modelId);
    m_models.erase(modelIt);
}

void ModelManager::unloadAllModels() {
    std::lock_guard<std::mutex> lock(m_mutex);

    for (const auto& [modelId, model] : m_models) {
        unloadModelTextures(model);
        unloadModelFromGraphicsLib(modelId);
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

int ModelManager::checkModelCache(const std::string& modelPath) {
    auto it = m_pathToId.find(modelPath);
    if (it != m_pathToId.end()) {
        return it->second;
    }
    return -1;
}

bool ModelManager::checkGraphicsLibInitialized() {
    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé dans ModelManager" << std::endl;
        return false;
    }
    return true;
}

Model3D ModelManager::createModelObject(int modelId) {
    Model3D model;
    model.modelId = modelId;
    model.scale = {1.0f, 1.0f, 1.0f};
    model.boundingBoxMin = {0.0f, 0.0f, 0.0f};
    model.boundingBoxMax = {0.0f, 0.0f, 0.0f};
    return model;
}

void ModelManager::loadTexturesForModel(Model3D& model, const std::vector<std::string>& texturePaths) {
    for (const auto& texturePath : texturePaths) {
        loadTextureForModel(model, texturePath);
    }
}

bool ModelManager::loadTextureForModel(Model3D& model, const std::string& texturePath) {
    if (texturePath.empty())
        return false;
    auto& textureManager = TextureManager::getInstance();
    int textureId = textureManager.loadTexture(texturePath);
    if (textureId >= 0) {
        model.textureIds.push_back(textureId);
        std::cout << "Texture " << texturePath << " chargée via TextureManager (ID: " << textureId << ")" << std::endl;
        return true;
    } else {
        std::cerr << "Avertissement: Échec du chargement de la texture " << texturePath << std::endl;
        return false;
    }
}

void ModelManager::registerModel(const std::string& modelPath, int modelId, const Model3D& model) {
    m_pathToId[modelPath] = modelId;
    m_models[modelId] = model;
}

void ModelManager::unloadModelTextures(const Model3D& model) {
    auto& textureManager = TextureManager::getInstance();
    for (int textureId : model.textureIds) {
        textureManager.unloadTexture(textureId);
    }
}

void ModelManager::unloadModelFromGraphicsLib(int modelId) {
    if (m_graphicsLib) {
        m_graphicsLib->UnloadModel3D(modelId);
    }
}

void ModelManager::removeModelPathReferences(int modelId) {
    for (auto it = m_pathToId.begin(); it != m_pathToId.end(); ) {
        if (it->second == modelId) {
            it = m_pathToId.erase(it);
        } else {
            ++it;
        }
    }
}

ModelManager::~ModelManager() {
    unloadAllModels();
}

bool ModelManager::validateModelForDrawing(int modelId) {
    if (!m_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé" << std::endl;
        return false;
    }
    auto it = m_models.find(modelId);
    if (it == m_models.end()) {
        std::cerr << "Erreur: Modèle ID " << modelId << " non trouvé" << std::endl;
        return false;
    }
    return true;
}
