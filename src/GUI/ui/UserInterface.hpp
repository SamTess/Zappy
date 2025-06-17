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
     * @brief Ajoute un message dans le journal
     * @param message Message à ajouter
     */
    void addLogMessage(const std::string& message);

    /**
     * @brief Efface tous les messages du journal
     */
    void clearLogs();

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
     * @brief Gère les événements de la souris
     */
    void handleMouseEvents();

private:
    // Référence à la bibliothèque graphique
    std::shared_ptr<IGuiLib> m_guiLib;

    // Factory pour la création et gestion des fenêtres
    std::unique_ptr<UIWindowFactory> m_windowFactory;

    // Dimensions de l'écran
    int m_screenWidth;
    int m_screenHeight;

    // État du glissement des fenêtres
    bool m_isDragging;
};

} // namespace GUI
