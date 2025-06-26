/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager
*/

#pragma once

#include <string>
#include <memory>
#include <map>
#include <mutex>
#include <iostream>
#include <utility>
#include <vector>
#include "../../../src/Shared/IGraphicsLib.hpp"
#include "../../../src/Shared/Common.hpp"

struct Model3D {
    int modelId;
    std::vector<int> textureIds;
    ZappyTypes::Vector3 scale;
    ZappyTypes::Vector3 boundingBoxMin;
    ZappyTypes::Vector3 boundingBoxMax;
};

typedef enum {
    TRIPY_TROPHY = 1,
    ESPRESSONA_SIGNORA = 2,
    FRIGO_CAMELO = 3,
    GARAMARAN = 4,
    LA_VACA_SATURNO = 5,
    TRALALERO_TRALALA = 6,
    TRULIMERO_TRULICINA = 7,
    TUNG_TUNG_TUNG_SAHUR = 8,
    ISLAND = 9,
    LABUBU = 10
} ModelType;

class ModelManager {
public:
    static ModelManager& getInstance() {
        static ModelManager instance;
        return instance;
    }
    void setGraphicsLib(std::shared_ptr<IGraphicsLib> graphicsLib);
    bool hasModelLibSet() const { return _graphicsLib != nullptr; }
    int loadModel(const std::string& modelPath, const std::string& texturePath = "");
    int loadModelWithTextures(const std::string& modelPath, const std::vector<std::string>& texturePaths);
    void drawModel(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color = ZappyTypes::Colors::Z_WHITE);
    void drawModel(int modelId, ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color = ZappyTypes::Colors::Z_WHITE);
    void drawModelEx(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Vector3 rotationAxis,
                    float rotationAngle, float scale = 1.0f, ZappyTypes::Color color = ZappyTypes::Colors::Z_WHITE);
    void unloadModel(int modelId);
    void unloadAllModels();
    bool hasModel(const std::string& modelPath) const;
    int getModelId(const std::string& modelPath) const;
    size_t getModelCount() const;
    std::pair<ZappyTypes::Vector3, ZappyTypes::Vector3>
        getBoundingBox(int modelId) const;

private:
    ModelManager() {};
    ~ModelManager();
    int checkModelCache(const std::string& modelPath);
    bool checkGraphicsLibInitialized();
    Model3D createModelObject(int modelId);
    Model3D loadTexturesForModel(Model3D model, const std::vector<std::string>& texturePaths);
    void registerModel(const std::string& modelPath, int modelId, const Model3D& model);
    void unloadModelFromGraphicsLib(int modelId);
    void removeModelPathReferences(int modelId);
    bool validateModelForDrawing(int modelId);

    std::shared_ptr<IGraphicsLib> _graphicsLib = nullptr;
    std::map<std::string, int> _pathToId;
    std::map<int, Model3D> _models;
    int _nextModelId = 1;
};
