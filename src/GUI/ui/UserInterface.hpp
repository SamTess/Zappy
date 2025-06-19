/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** User Interface implementation
*/

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include "UIWindowFactory.hpp"
#include "../shared/GameData.hpp"
#include "../../Shared/IGuiLib.hpp"

namespace GUI {

class UserInterface {
public:
    /**
     * @brief Constructeur de l'interface utilisateur
     * @param guiLib Bibliothèque graphique à utiliser
     */
    explicit UserInterface(std::shared_ptr<IGuiLib> guiLib);
    ~UserInterface() = default;

    /**
     * @brief Initialise l'interface utilisateur
     * @param screenWidth Largeur de l'écran
     * @param screenHeight Hauteur de l'écran
     */
    void initialize(int screenWidth, int screenHeight);

    /**
     * @brief Effectue le rendu de l'interface
     */
    void render();

    /**
     * @brief Met à jour les données de l'interface
     * @param gameData Données du jeu
     */
    void updateData(const GameData& gameData);

    /**
     * @brief Définit la case sélectionnée
     * @param x Coordonnée X
     * @param y Coordonnée Y
     */
    void setSelectedTile(int x, int y);

    /**
     * @brief Ajoute un message de diffusion
     * @param team Équipe émettrice
     * @param message Message diffusé
     */
    void addBroadcast(const std::string& team, const std::string& message);

    /**
     * @brief Définit le mode de vue
     * @param mode Indice du mode de vue
     */
    void setViewMode(int mode);

    /**
     * @brief Affiche ou masque une fenêtre
     * @param windowId Identifiant de la fenêtre
     * @param visible true pour afficher, false pour masquer
     * @return true si l'opération a réussi
     */
    bool toggleWindowVisibility(const std::string& windowId, bool visible);

    /**
     * @brief Gère les événements de la souris
     * @return true si l'UI a capturé l'événement de souris
     */
    bool handleMouseEvents();

    /**
     * @brief Vérifie si la souris est sur une fenêtre de l'interface
     * @return true si la souris est sur une fenêtre
     */
    bool isMouseOverUI() const;

    /**
     * @brief Vérifie si l'UI a traité le dernier événement de souris
     * @return true si l'UI a capturé l'événement
     */
    bool hasHandledMouseEvent() const;

private:
    /**
     * @brief Gère les interactions de souris sur les éléments d'interface
     * @param mousePosition Position actuelle de la souris
     */
    void handleUIMouseInteraction(const ZappyTypes::Vector2& mousePosition);

    /**
     * @brief Démarre le glissement d'une fenêtre si possible
     * @param mousePosition Position actuelle de la souris
     */
    void startDraggingIfPossible(const ZappyTypes::Vector2& mousePosition);

    /**
     * @brief Gère la fin du glissement si nécessaire
     */
    void handleDragEndIfNeeded();

    // Référence à la bibliothèque graphique
    std::shared_ptr<IGuiLib> m_guiLib;

    // Factory pour la création et gestion des fenêtres
    std::shared_ptr<UIWindowFactory> m_windowFactory;

    // Dimensions de l'écran
    int m_screenWidth;
    int m_screenHeight;

    // État du glissement des fenêtres
    bool m_isDragging;

    // Indique si l'UI a capturé le dernier clic de souris
    bool m_mouseCapture;
};

} // namespace GUI
