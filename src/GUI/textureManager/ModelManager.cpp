/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager
*/

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "ModelManager.hpp"

void ModelManager::setGraphicsLib(std::shared_ptr<IGraphicsLib> graphicsLib) {
    _graphicsLib = graphicsLib;
}

int ModelManager::loadModel(const std::string& modelPath, const std::string& texturePath) {
    auto it = _pathToId.find(modelPath);
    if (it != _pathToId.end())
        return it->second;
    if (!_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé dans ModelManager" << std::endl;
        return -1;
    }

    try {
        int modelId;
        if (!texturePath.empty())
            modelId = _graphicsLib->LoadModelWithTexture(modelPath, texturePath);
        else
            modelId = _graphicsLib->LoadModel3D(modelPath);
        if (modelId < 0) {
            std::cerr << "Erreur: Échec du chargement du modèle " << modelPath << std::endl;
            return -1;
        }
        Model3D model = createModelObject(modelId);
        if (!texturePath.empty()) {
            auto [success, updatedModel] = std::make_pair(false, model);
            if (success)
                model = updatedModel;
        }
        registerModel(modelPath, modelId, model);
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle " << modelPath << ": " << e.what() << std::endl;
        return -1;
    }
}

int ModelManager::loadModelWithTextures(const std::string& modelPath, const std::vector<std::string>& texturePaths) {
    int cachedModelId = checkModelCache(modelPath);
    if (cachedModelId != -1)
        return cachedModelId;
    if (!checkGraphicsLibInitialized())
        return -1;
    try {
        int modelId;
        if (!texturePaths.empty())
            modelId = _graphicsLib->LoadModelWithTexture(modelPath, texturePaths[0]);
        else
            modelId = _graphicsLib->LoadModel3D(modelPath);
        if (modelId < 0) {
            std::cerr << "Erreur: Échec du chargement du modèle " << modelPath << std::endl;
            return -1;
        }
        Model3D model = createModelObject(modelId);
        model = loadTexturesForModel(model, texturePaths);
        registerModel(modelPath, modelId, model);
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle " << modelPath << ": " << e.what() << std::endl;
        return -1;
    }
}

void ModelManager::drawModel(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color) {
    if (!validateModelForDrawing(modelId))
        return;
    _graphicsLib->DrawModel3D(modelId, position, 1.0f, color);
}

void ModelManager::drawModel(int modelId, ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color) {
    if (!validateModelForDrawing(modelId))
        return;
    _graphicsLib->DrawModel3D(modelId, position, scale, color);
}

void ModelManager::drawModelEx(int modelId, ZappyTypes::Vector3 position,
    ZappyTypes::Vector3 rotationAxis,
    float rotationAngle, float scale, ZappyTypes::Color color) {

    if (!validateModelForDrawing(modelId))
        return;
    _graphicsLib->DrawModelEx(modelId, position, rotationAxis, rotationAngle, scale, color);
}

void ModelManager::unloadModel(int modelId) {
    auto modelIt = _models.find(modelId);

    if (modelIt == _models.end()) {
        std::cerr << "Tentative de libération d'un modèle inexistant (ID: " << modelId << ")" << std::endl;
        return;
    }
    unloadModelFromGraphicsLib(modelId);
    removeModelPathReferences(modelId);
    _models.erase(modelIt);
}

void ModelManager::unloadAllModels() {
    for (const auto& [modelId, model] : _models)
        unloadModelFromGraphicsLib(modelId);
    _models.clear();
    _pathToId.clear();
}

bool ModelManager::hasModel(const std::string& modelPath) const {
    return _pathToId.find(modelPath) != _pathToId.end();
}

int ModelManager::getModelId(const std::string& modelPath) const {
    auto it = _pathToId.find(modelPath);
    if (it != _pathToId.end())
        return it->second;
    return -1;
}

size_t ModelManager::getModelCount() const {
    return _models.size();
}

std::pair<ZappyTypes::Vector3, ZappyTypes::Vector3> ModelManager::getBoundingBox(int modelId) const {
    auto it = _models.find(modelId);
    if (it != _models.end())
        return {it->second.boundingBoxMin, it->second.boundingBoxMax};
    return {ZappyTypes::Vector3{0, 0, 0}, ZappyTypes::Vector3{0, 0, 0}};
}

int ModelManager::checkModelCache(const std::string& modelPath) {
    auto it = _pathToId.find(modelPath);
    if (it != _pathToId.end())
        return it->second;
    return -1;
}

bool ModelManager::checkGraphicsLibInitialized() {
    if (!_graphicsLib) {
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

Model3D ModelManager::loadTexturesForModel(Model3D model, const std::vector<std::string>& texturePaths) {
    for (const auto& texturePath : texturePaths) {
        (void)texturePath;
        auto [success, updatedModel] = std::make_pair(false, model);
        if (success)
            model = updatedModel;
    }
    return model;
}

void ModelManager::registerModel(const std::string& modelPath, int modelId, const Model3D& model) {
    _pathToId[modelPath] = modelId;
    _models[modelId] = model;
}



void ModelManager::unloadModelFromGraphicsLib(int modelId) {
    if (_graphicsLib)
        _graphicsLib->UnloadModel3D(modelId);
}

void ModelManager::removeModelPathReferences(int modelId) {
    for (auto it = _pathToId.begin(); it != _pathToId.end(); ++it) {
        if (it->second == modelId) {
            it = _pathToId.erase(it);
            return;
        }
    }
}

ModelManager::~ModelManager() {
    unloadAllModels();
}

bool ModelManager::validateModelForDrawing(int modelId) {
    if (!_graphicsLib) {
        std::cerr << "Erreur: GraphicsLib non initialisé" << std::endl;
        return false;
    }
    auto it = _models.find(modelId);
    if (it == _models.end()) {
        std::cerr << "Erreur: Modèle ID " << modelId << " non trouvé" << std::endl;
        return false;
    }
    return true;
}
