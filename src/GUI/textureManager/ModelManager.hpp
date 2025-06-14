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
#include "../../../src/Shared/IGraphicsLib.hpp"
#include "../../../src/Shared/Common.hpp"
#include "TextureManager.hpp"

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
     * @brief Vérifie si la bibliothèque graphique a été initialisée
     * @return true si initialisée, false sinon
     */
    bool hasModelLibSet() const { return m_graphicsLib != nullptr; }

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
    ModelManager() : m_textureManager(TextureManager::getInstance()) {}
    ~ModelManager();

    // Empêcher la copie ou l'assignation
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    /**
     * @brief Vérifie si un modèle existe déjà dans le cache
     * @param modelPath Chemin d'accès au fichier du modèle
     * @return ID du modèle s'il existe, -1 sinon
     */
    int checkModelCache(const std::string& modelPath);

    /**
     * @brief Vérifie si la bibliothèque graphique est initialisée
     * @return true si initialisée, false sinon
     */
    bool checkGraphicsLibInitialized();

    /**
     * @brief Crée et initialise un objet modèle 3D
     * @param modelId ID du modèle chargé
     * @return Objet Model3D initialisé
     */
    Model3D createModelObject(int modelId);

    /**
     * @brief Charge les textures pour un modèle et retourne le modèle modifié
     * @param model Objet modèle à modifier
     * @param texturePaths Liste des chemins d'accès aux textures
     * @return Modèle avec les textures chargées
     */
    Model3D loadTexturesForModel(Model3D model, const std::vector<std::string>& texturePaths);

    /**
     * @brief Charge une texture unique pour un modèle et retourne le modèle modifié
     * @param model Objet modèle à modifier
     * @param texturePath Chemin d'accès à la texture
     * @return std::pair<bool, Model3D> Résultat du chargement et modèle modifié
     */
    std::pair<bool, Model3D> loadTextureForModel(Model3D model, const std::string& texturePath);

    /**
     * @brief Enregistre le modèle dans les structures de données internes
     * @param modelPath Chemin d'accès au fichier du modèle
     * @param modelId ID du modèle
     * @param model Objet modèle à enregistrer
     */
    void registerModel(const std::string& modelPath, int modelId, const Model3D& model);

    /**
     * @brief Décharge les textures associées à un modèle
     * @param model Modèle dont les textures doivent être déchargées
     */
    void unloadModelTextures(const Model3D& model);

    /**
     * @brief Décharge un modèle spécifique de la bibliothèque graphique
     * @param modelId ID du modèle à décharger
     */
    void unloadModelFromGraphicsLib(int modelId);

    /**
     * @brief Supprime les références d'un modèle dans le dictionnaire de chemins
     * @param modelId ID du modèle à supprimer
     */
    void removeModelPathReferences(int modelId);

    /**
     * @brief Vérifie si un modèle peut être dessiné (existe et lib graphique initialisée)
     * @param modelId ID du modèle à vérifier
     * @return true si le modèle peut être dessiné, false sinon
     */
    bool validateModelForDrawing(int modelId);

    TextureManager& m_textureManager;                // Instance de TextureManager pour gérer les textures associées

    std::shared_ptr<IGraphicsLib> m_graphicsLib = nullptr;
    std::map<std::string, int> m_pathToId;           // Associe les chemins aux IDs
    std::map<int, Model3D> m_models;                 // Stocke les modèles avec leurs propriétés
    int m_nextModelId = 1;                           // ID à attribuer au prochain modèle
    mutable std::mutex m_mutex;                      // Mutex pour thread-safety
};
