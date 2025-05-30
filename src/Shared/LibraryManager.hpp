/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** LibraryManager
*/

#pragma once

#include <string>
#include <memory>
#include <functional>
#include "DLLoader.hpp"
#include "IGraphicsLib.hpp"
#include "IGuiLib.hpp"

class LibraryManager {
public:
    /**
     * @brief Obtient l'instance singleton du gestionnaire de bibliothèques
     * @return Instance du LibraryManager
     */
    static LibraryManager& getInstance() {
        static LibraryManager instance;
        return instance;
    }

    /**
     * @brief Charge une bibliothèque graphique
     * @param libPath Chemin vers la bibliothèque graphique
     * @param creatorFunction Nom de la fonction de création (par défaut "createGraphicsLib")
     * @return true si chargée avec succès, false sinon
     */
    bool loadGraphicsLib(const std::string& libPath, const std::string& creatorFunction = "createGraphicsLib") {
        try {
            _graphicsLoader = std::make_unique<DLLoader<IGraphicsLib>>(libPath);
            _graphicsLib = _graphicsLoader->getInstance(creatorFunction);
            return true;
        } catch (const std::exception& e) {
            _lastError = e.what();
            return false;
        }
    }

    /**
     * @brief Charge une bibliothèque GUI
     * @param libPath Chemin vers la bibliothèque GUI
     * @param creatorFunction Nom de la fonction de création (par défaut "createGuiLib")
     * @return true si chargée avec succès, false sinon
     */
    bool loadGuiLib(const std::string& libPath, const std::string& creatorFunction = "createGuiLib") {
        try {
            _guiLoader = std::make_unique<DLLoader<IGuiLib>>(libPath);
            _guiLib = _guiLoader->getInstance(creatorFunction);
            return true;
        } catch (const std::exception& e) {
            _lastError = e.what();
            return false;
        }
    }

    /**
     * @brief Obtient la bibliothèque graphique chargée
     * @return Référence vers la bibliothèque graphique
     */
    IGraphicsLib& getGraphicsLib() {
        if (!_graphicsLib) {
            throw std::runtime_error("Bibliothèque graphique non chargée");
        }
        return *_graphicsLib;
    }

    /**
     * @brief Obtient la bibliothèque GUI chargée
     * @return Référence vers la bibliothèque GUI
     */
    IGuiLib& getGuiLib() {
        if (!_guiLib) {
            throw std::runtime_error("Bibliothèque GUI non chargée");
        }
        return *_guiLib;
    }

    /**
     * @brief Obtient la dernière erreur générée
     * @return Message d'erreur
     */
    const std::string& getLastError() const {
        return _lastError;
    }

private:
    LibraryManager() = default;
    LibraryManager(const LibraryManager&) = delete;
    LibraryManager& operator=(const LibraryManager&) = delete;

    std::unique_ptr<DLLoader<IGraphicsLib>> _graphicsLoader;
    std::unique_ptr<DLLoader<IGuiLib>> _guiLoader;
    std::unique_ptr<IGraphicsLib, std::function<void(IGraphicsLib*)>> _graphicsLib;
    std::unique_ptr<IGuiLib, std::function<void(IGuiLib*)>> _guiLib;
    std::string _lastError;
};
