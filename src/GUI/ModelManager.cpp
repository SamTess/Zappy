/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager
*/

#include "ModelManager.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

ModelManager::ModelManager()
    : m_modelLoaded(false),
      m_model(""),
      m_modelPosition({0.0f, 0.0f, 0.0f}),
      m_modelScale(1.0f),
      m_modelRotation(0.0f),
      m_showModel(true),
      m_randomPlacement(false),
      m_currentModelPath(""),
      m_currentModelName(""),
      m_currentModelIndex(0),
      m_globalModelScale(1.0f) {
    m_modelPaths = {
        "assets/models/14-girl-obj/girl.obj",
        "assets/models/bugatti.obj",
        "assets/models/cartoon.obj"
    };

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int ModelManager::GetRandomValue(int min, int max) {
    return min + (std::rand() % (max - min + 1));
}

bool ModelManager::loadModel3D(std::shared_ptr<IGraphicsLib> graphics, const std::string& modelPath) {
    try {
        std::cout << "Chargement du modèle 3D: " << modelPath << std::endl;
        graphics->LoadModel3D(modelPath);
        m_modelLoaded = true;
        m_currentModelPath = modelPath;

        size_t lastSlash = modelPath.find_last_of('/');
        if (lastSlash != std::string::npos) {
            m_currentModelName = modelPath.substr(lastSlash + 1);
        } else {
            m_currentModelName = modelPath;
        }

        std::cout << "Modèle 3D chargé avec succès!" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle 3D: " << e.what() << std::endl;
        m_modelLoaded = false;
        return false;
    }
}

bool ModelManager::loadGirlModel(std::shared_ptr<IGraphicsLib> graphics) {
    std::string modelPath = "assets/models/14-girl-obj/girl.obj";

    try {
        std::cout << "Chargement du modèle 3D de la fille: " << modelPath << std::endl;
        std::cout << "Tentative de chargement depuis le chemin : " << modelPath << std::endl;

        graphics->LoadModel3D(modelPath);
        m_modelLoaded = true;
        m_currentModelPath = modelPath;
        m_currentModelName = "girl.obj";
        m_modelPosition = {0.0f, 0.0f, 0.0f};
        m_modelScale = 5.0f;
        m_modelRotation = 0.0f;

        std::cout << "Modèle 3D de la fille chargé avec succès!" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle 3D de la fille: " << e.what() << std::endl;
        try {
            std::string absPath = "/home/alex/Zappy/assets/models/14-girl-obj/girl.obj";
            std::cout << "Tentative avec un chemin absolu: " << absPath << std::endl;
            graphics->LoadModel3D(absPath);
            m_modelLoaded = true;
            m_currentModelPath = absPath;
            m_currentModelName = "girl.obj";
            m_modelPosition = {0.0f, 0.0f, 0.0f};
            m_modelScale = 0.1f;
            m_modelRotation = 0.0f;
            std::cout << "Modèle 3D de la fille chargé avec succès avec chemin absolu!" << std::endl;
            return true;
        } catch (const std::exception& e2) {
            std::cerr << "Échec avec le chemin absolu: " << e2.what() << std::endl;
            m_modelLoaded = false;
            return false;
        }
    }
}

void ModelManager::loadNextModel(std::shared_ptr<IGraphicsLib> graphics) {
    m_currentModelIndex = (m_currentModelIndex + 1) % m_modelPaths.size();
    loadModel3D(graphics, m_modelPaths[m_currentModelIndex]);
}

void ModelManager::initializeRandomModels(std::shared_ptr<IGraphicsLib> graphics) {
    m_models.clear();
    m_modelPositions.clear();
    m_modelColors.clear();
    m_modelScales.clear();

    if (!m_modelLoaded) {
        loadModel3D(graphics, m_modelPaths[2]);
    }

    for (size_t i = 0; i < static_cast<size_t>(NUM_RANDOM_MODELS); i++) {
        ZappyTypes::Vector3 position = {
            static_cast<float>(GetRandomValue(-10, 10)),
            0.5f,
            static_cast<float>(GetRandomValue(-10, 10))
        };
        m_modelPositions.push_back(position);

        ZappyTypes::Color color = {
            static_cast<unsigned char>(GetRandomValue(100, 255)),
            static_cast<unsigned char>(GetRandomValue(100, 255)),
            static_cast<unsigned char>(GetRandomValue(100, 255)),
            255
        };
        m_modelColors.push_back(color);

        float scale = GetRandomValue(5, 15) / 50.0f;
        m_modelScales.push_back(scale);
    }
}

void ModelManager::drawModel(std::shared_ptr<IGraphicsLib> graphics) {
    if (m_modelLoaded && m_showModel) {
        ZappyTypes::Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
        float fixedRotation = 0.0f;
        graphics->DrawModelEx(m_modelPosition, rotationAxis, fixedRotation, m_modelScale);

        if (m_randomPlacement) {
            for (size_t i = 0; i < static_cast<size_t>(NUM_RANDOM_MODELS) && i < m_modelPositions.size(); i++) {
                graphics->DrawModel3D(m_modelPositions[i], m_modelScales[i], m_modelColors[i]);
            }
        }
    }
}

void ModelManager::updateModelRotation(float amount) {
    m_modelRotation += amount;
    if (m_modelRotation > 360.0f) {
        m_modelRotation = 0.0f;
    }
}
