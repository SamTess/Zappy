/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TextureManager - Gestionnaire centralisé de textures
*/

#pragma once

#include <string>
#include <memory>
#include <map>
#include <mutex>
#include <iostream>
#include "../../../src/Shared/IGraphicsLib.hpp"

/**
 * @brief Gestionnaire de textures singleton qui implémente un pattern de pool de ressources
 * pour éviter de charger plusieurs fois les mêmes textures.
 */
class TextureManager {
public:
    /**
     * @brief Obtient l'instance unique du gestionnaire de textures
     * @return L'instance du TextureManager
     */
    static TextureManager& getInstance() {
        static TextureManager instance;
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
    bool hasTextureLibSet() const { return m_graphicsLib != nullptr; }

    /**
     * @brief Charge une texture à partir d'un chemin
     * Si la texture existe déjà dans le cache, elle est retournée directement.
     *
     * @param path Chemin d'accès à l'image de la texture
     * @return ID de la texture chargée
     */
    int loadTexture(const std::string& path);

    /**
     * @brief Utilise une texture préchargée pour dessin
     * @param textureId ID de la texture à dessiner
     * @param x Position X à l'écran
     * @param y Position Y à l'écran
     */
    void drawTexture(int textureId, int x, int y);

    /**
     * @brief Libère une texture spécifique
     * @param textureId ID de la texture à libérer
     */
    void unloadTexture(int textureId);

    /**
     * @brief Libère toutes les textures en mémoire
     */
    void unloadAllTextures();

    /**
     * @brief Vérifie si une texture est déjà chargée
     * @param path Chemin d'accès à l'image de la texture
     * @return true si la texture existe, false sinon
     */
    bool hasTexture(const std::string& path) const;

    /**
     * @brief Obtient l'ID d'une texture déjà chargée
     * @param path Chemin d'accès à l'image de la texture
     * @return ID de la texture ou -1 si non trouvée
     */
    int getTextureId(const std::string& path) const;

    /**
     * @brief Obtient le nombre de textures actuellement chargées
     * @return Nombre de textures en cache
     */
    size_t getTextureCount() const;

private:
    TextureManager() = default;
    ~TextureManager();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::shared_ptr<IGraphicsLib> m_graphicsLib = nullptr;
    std::map<std::string, int> m_pathToId;       // Associe les chemins aux IDs
    std::map<int, std::string> m_idToPath;       // Association inverse
    int m_nextTextureId = 1;                     // ID à attribuer à la prochaine texture
    mutable std::mutex m_mutex;                  // Mutex pour thread-safety
};
