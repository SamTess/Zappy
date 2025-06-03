/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager
*/

#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/Common.hpp"
#include "Constants.hpp"

struct ModelInstance {
    std::string modelPath;
    ZappyTypes::Vector3 position;
    ZappyTypes::Vector3 scale;
    ZappyTypes::Vector3 rotation;
    ZappyTypes::Color color;
};

class ModelManager {
public:
    ModelManager();
    ~ModelManager() = default;

    bool loadModel3D(std::shared_ptr<IGraphicsLib> graphics, const std::string& modelPath);
    bool loadGirlModel(std::shared_ptr<IGraphicsLib> graphics);
    void loadNextModel(std::shared_ptr<IGraphicsLib> graphics);
    void initializeRandomModels(std::shared_ptr<IGraphicsLib> graphics);
    void drawModel(std::shared_ptr<IGraphicsLib> graphics);
    void updateModelRotation(float amount);

    bool isModelLoaded() const { return m_modelLoaded; }
    bool isShowModel() const { return m_showModel; }
    void setShowModel(bool show) { m_showModel = show; }
    bool isRandomPlacement() const { return m_randomPlacement; }
    void setRandomPlacement(bool random) { m_randomPlacement = random; }

    ZappyTypes::Vector3& position() { return m_modelPosition; }
    float& scale() { return m_modelScale; }
    float& rotation() { return m_modelRotation; }
    float& globalScale() { return m_globalModelScale; }
    std::vector<ZappyTypes::Vector3>& positions() { return m_modelPositions; }
    std::vector<ZappyTypes::Color>& colors() { return m_modelColors; }
    std::vector<float>& scales() { return m_modelScales; }
    std::vector<ModelInstance>& models() { return m_models; }
    std::vector<std::string>& paths() { return m_modelPaths; }
    int& currentIndex() { return m_currentModelIndex; }
    std::string& currentName() { return m_currentModelName; }

    int GetRandomValue(int min, int max);

private:
    bool m_modelLoaded;
    std::string m_model;
    ZappyTypes::Vector3 m_modelPosition;
    float m_modelScale;
    float m_modelRotation;
    bool m_showModel;
    bool m_randomPlacement;
    std::string m_currentModelPath;
    std::string m_currentModelName;

    std::vector<std::string> m_modelPaths;
    int m_currentModelIndex;
    float m_globalModelScale;

    std::vector<ModelInstance> m_models;
    std::vector<ZappyTypes::Vector3> m_modelPositions;
    std::vector<ZappyTypes::Color> m_modelColors;
    std::vector<float> m_modelScales;
};
