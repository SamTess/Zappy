/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Interface for UI Windows
*/

#pragma once

#include <memory>
#include "../../shared/GameData.hpp"
#include "../../../Shared/IGuiLib.hpp"

namespace GUI {

class IUIWindow {
public:
    virtual ~IUIWindow() = default;

    /**
     * @brief Initialise la fenêtre avec les paramètres nécessaires
     * @param position Position de la fenêtre
     * @param dimensions Dimensions de la fenêtre
     */
    virtual void initialize(const ZappyTypes::Vector2& position,
                           const ZappyTypes::Vector2& dimensions) = 0;

    /**
     * @brief Effectue le rendu de la fenêtre
     * @param guiLib Bibliothèque GUI à utiliser pour le rendu
     * @return true si la fenêtre doit rester ouverte, false sinon
     */
    virtual bool render() = 0;

    /**
     * @brief Met à jour les données de la fenêtre
     * @param gameData Données du jeu actualisées
     */
    virtual void updateData(const GameData& gameData) = 0;

    /**
     * @brief Déplace la fenêtre
     * @param position Nouvelle position de la fenêtre
     */
    virtual void setPosition(const ZappyTypes::Vector2& position) = 0;

    /**
     * @brief Récupère la position actuelle de la fenêtre
     * @return Position actuelle de la fenêtre
     */
    virtual ZappyTypes::Vector2 getPosition() const = 0;

    /**
     * @brief Récupère les dimensions de la fenêtre
     * @return Dimensions de la fenêtre
     */
    virtual ZappyTypes::Vector2 getDimensions() const = 0;

    /**
     * @brief Indique si la fenêtre est visible
     * @return true si la fenêtre est visible, false sinon
     */
    virtual bool isVisible() const = 0;

    /**
     * @brief Définit la visibilité de la fenêtre
     * @param visible État de visibilité à appliquer
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief Bascule l'état de visibilité de la fenêtre
     */
    virtual void toggleVisibility() = 0;

    /**
     * @brief Gère le début du glissement de la fenêtre
     * @param mousePosition Position actuelle de la souris
     * @return true si le glissement a commencé, false sinon
     */
    virtual bool startDragging(const ZappyTypes::Vector2& mousePosition) = 0;

    /**
     * @brief Met à jour la position de la fenêtre pendant le glissement
     * @param mousePosition Position actuelle de la souris
     */
    virtual void updateDragging(const ZappyTypes::Vector2& mousePosition) = 0;

    /**
     * @brief Termine le glissement de la fenêtre
     */
    virtual void stopDragging() = 0;

    /**
     * @brief Vérifie si la fenêtre est en cours de glissement
     * @return true si la fenêtre est en cours de glissement, false sinon
     */
    virtual bool isDragging() const = 0;

    /**
     * @brief Vérifie si une position est à l'intérieur de la fenêtre
     * @param position Position à vérifier
     * @return true si la position est à l'intérieur de la fenêtre, false sinon
     */
    virtual bool isPositionInWindow(const ZappyTypes::Vector2& position) const = 0;
};

} // namespace GUI
