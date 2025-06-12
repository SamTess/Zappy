/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManagerAdapter - Adaptateur pour le singleton ModelManager
*/

#pragma once

#include <memory>
#include "ModelManager.hpp"

namespace Zappy {

/**
 * @brief Adaptateur qui permet de gérer ModelManager comme un std::shared_ptr
 * Cette classe encapsule le singleton ModelManager pour l'utiliser avec des shared_ptr
 */
class ModelManagerAdapter {
private:
    ModelManager& managerRef;

public:
    /**
     * @brief Constructeur qui encapsule une référence au singleton ModelManager
     */
    explicit ModelManagerAdapter() : managerRef(ModelManager::getInstance()) {}

    /**
     * @brief Obtient une référence au singleton ModelManager encapsulé
     * @return Référence à ModelManager
     */
    ModelManager& getManager() { return managerRef; }

    /**
     * @brief Fonction utilitaire pour créer un shared_ptr à ModelManager
     * @return shared_ptr vers ModelManagerAdapter
     */
    static std::shared_ptr<ModelManagerAdapter> createShared() {
        return std::make_shared<ModelManagerAdapter>();
    }

    // Délégation des méthodes principales de ModelManager

    /**
     * @brief Définit la bibliothèque graphique à utiliser
     * @param graphicsLib Pointeur partagé vers une implémentation de IGraphicsLib
     */
    void setGraphicsLib(std::shared_ptr<IGraphicsLib> graphicsLib) {
        managerRef.setGraphicsLib(graphicsLib);
    }

    // Ajoutez ici d'autres méthodes déléguées si nécessaires
    // Si vous avez besoin d'accéder directement au ModelManager, utilisez getManager()
};

} // namespace Zappy
