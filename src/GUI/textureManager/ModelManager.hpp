/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager - Gestionnaire centralisé de modèles 3D
*/

#pragma once

#include <string>
#include <memory>
#include <map>
#include <mutex>
#include <iostream>
#include <utility>
#include <vector>
#include "../../Shared/IGraphicsLib.hpp"
#include "../../Shared/Common.hpp"

/**
 * @brief Structure représentant un modèle 3D avec ses textures associées
 */
struct Model3D {
    int modelId;                       // ID du modèle
    std::vector<int> textureIds;       // IDs des textures associées
    ZappyTypes::Vector3 scale;         // Échelle du modèle
    ZappyTypes::Vector3 boundingBoxMin;// Point minimum de la boîte englobante
    ZappyTypes::Vector3 boundingBoxMax;// Point maximum de la boîte englobante
};

/**
 * @brief Gestionnaire de modèles 3D singleton qui implémente un pattern de pool de ressources
 * pour éviter de charger plusieurs fois les mêmes modèles.
 */
class ModelManager {
public:
    /**
     * @brief Obtient l'instance unique du gestionnaire de modèles
     * @return L'instance du ModelManager
     */
    static ModelManager& getInstance() {
        static ModelManager instance;
        return instance;
    }

    /**
     * @brief Définit la bibliothèque graphique à utiliser
     * @param graphicsLib Pointeur partagé vers une implémentation de IGraphicsLib
     */
    void setGraphicsLib(std::shared_ptr<IGraphicsLib> graphicsLib);

    /**
     * @brief Charge un modèle 3D à partir d'un chemin
     * Si le modèle existe déjà dans le cache, il est retourné directement.
     *
     * @param modelPath Chemin d'accès au fichier du modèle
     * @param texturePath Chemin d'accès à la texture principale (optionnel)
     * @return ID du modèle chargé ou -1 si échec
     */
    int loadModel(const std::string& modelPath, const std::string& texturePath = "");

    /**
     * @brief Charge un modèle avec plusieurs textures
     *
     * @param modelPath Chemin d'accès au fichier du modèle
     * @param texturePaths Liste des chemins d'accès aux textures
     * @return ID du modèle chargé ou -1 si échec
     */
    int loadModelWithTextures(const std::string& modelPath, const std::vector<std::string>& texturePaths);

    /**
     * @brief Dessine un modèle 3D à une position donnée
     *
     * @param modelId ID du modèle à dessiner
     * @param position Position du modèle dans l'espace 3D
     * @param color Couleur de teinte (blanc par défaut)
     */
    void drawModel(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Color color = ZappyTypes::Colors::Z_WHITE);

    /**
     * @brief Dessine un modèle 3D avec rotation
     *
     * @param modelId ID du modèle à dessiner
     * @param position Position du modèle
     * @param rotationAxis Axe de rotation
     * @param rotationAngle Angle de rotation en degrés
     * @param scale Échelle du modèle (1.0f par défaut)
     */
    void drawModelEx(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Vector3 rotationAxis,
                    float rotationAngle, float scale = 1.0f);

    /**
     * @brief Libère un modèle 3D spécifique et ses textures associées
     * @param modelId ID du modèle à libérer
     */
    void unloadModel(int modelId);

    /**
     * @brief Libère tous les modèles 3D en mémoire
     */
    void unloadAllModels();

    /**
     * @brief Vérifie si un modèle est déjà chargé
     * @param modelPath Chemin d'accès au fichier du modèle
     * @return true si le modèle existe, false sinon
     */
    bool hasModel(const std::string& modelPath) const;

    /**
     * @brief Obtient l'ID d'un modèle déjà chargé
     * @param modelPath Chemin d'accès au fichier du modèle
     * @return ID du modèle ou -1 si non trouvé
     */
    int getModelId(const std::string& modelPath) const;

    /**
     * @brief Obtient le nombre de modèles actuellement chargés
     * @return Nombre de modèles en cache
     */
    size_t getModelCount() const;

    /**
     * @brief Obtient la boîte englobante d'un modèle
     * @param modelId ID du modèle
     * @return Paire de vecteurs min et max de la boîte englobante
     */
    std::pair<ZappyTypes::Vector3, ZappyTypes::Vector3>
        getBoundingBox(int modelId) const;

private:
    ModelManager() = default;
    ~ModelManager();

    // Empêcher la copie ou l'assignation
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    std::shared_ptr<IGraphicsLib> m_graphicsLib = nullptr;
    std::map<std::string, int> m_pathToId;           // Associe les chemins aux IDs
    std::map<int, Model3D> m_models;                 // Stocke les modèles avec leurs propriétés
    int m_nextModelId = 1;                           // ID à attribuer au prochain modèle
    mutable std::mutex m_mutex;                      // Mutex pour thread-safety
};
