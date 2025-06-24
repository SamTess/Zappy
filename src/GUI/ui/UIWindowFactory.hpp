/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UIWindowFactory - Factory pour créer les différentes fenêtres UI
*/

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "windows/IUIWindow.hpp"
#include "../gameController/GameState.hpp"
#include "../../Shared/IGuiLib.hpp"

class NetworkManager;

namespace GUI {

/**
 * @brief Factory pour créer et gérer les fenêtres de l'interface utilisateur
 */
class UIWindowFactory : public std::enable_shared_from_this<UIWindowFactory> {
public:
    /**
     * @brief Constructeur
     * @param guiLib Pointeur vers la bibliothèque GUI
     */
    explicit UIWindowFactory(std::shared_ptr<IGuiLib> guiLib);
    ~UIWindowFactory() = default;

    /**
     * @brief Définit le NetworkManager pour permettre l'envoi de commandes
     * @param networkManager Pointeur vers le NetworkManager
     */
    void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);

    /**
     * @brief Crée toutes les fenêtres nécessaires pour l'interface utilisateur
     * @param screenWidth Largeur de l'écran
     * @param screenHeight Hauteur de l'écran
     */
    void createAllWindows(int screenWidth, int screenHeight);

    /**
     * @brief Obtient une fenêtre par son identifiant
     * @param windowId Identifiant de la fenêtre
     * @return Pointeur partagé vers la fenêtre
     */
    std::shared_ptr<IUIWindow> getWindow(const std::string& windowId);

    /**
     * @brief Rend toutes les fenêtres visibles
     */
    void renderAllWindows();

    /**
     * @brief Mise à jour des données pour toutes les fenêtres avec le GameState
     * @param gameState État actuel du jeu
     * @param mapWidth Largeur de la carte
     * @param mapHeight Hauteur de la carte
     * @param gameTime Temps de jeu écoulé
     * @param frequency Fréquence du jeu
     * @param gameTick Tick de jeu actuel
     */
    void updateAllWindows(std::shared_ptr<const GameState> gameState,
                         int mapWidth, int mapHeight,
                         float gameTime, int frequency, int gameTick);

    /**
     * @brief Définit la case sélectionnée
     * @param x Coordonnée X
     * @param y Coordonnée Y
     */
    void setSelectedTile(int x, int y);

    /**
     * @brief Définit le joueur sélectionné
     * @param playerId Identifiant du joueur sélectionné
     */
    void setSelectedPlayer(int playerId);


    /**
     * @brief Obtient le NetworkManager pour l'envoi de commandes
     * @return Pointeur partagé vers le NetworkManager
     */
    std::shared_ptr<NetworkManager> getNetworkManager() const;

    /**
     * @brief Définit le mode de vue
     * @param mode Indice du mode de vue
     */
    void setViewMode(int mode);

    /**
     * @brief Gère le début du glissement d'une fenêtre
     * @param mousePosition Position de la souris
     * @return true si une fenêtre a commencé à être déplacée, false sinon
     */
    bool handleWindowDragging(const ZappyTypes::Vector2& mousePosition);

    /**
     * @brief Met à jour la position des fenêtres en cours de glissement
     * @param mousePosition Position actuelle de la souris
     */
    void updateWindowDragging(const ZappyTypes::Vector2& mousePosition);

    /**
     * @brief Arrête le glissement des fenêtres
     */
    void stopWindowDragging();

    /**
     * @brief Vérifie si la souris est sur une fenêtre
     * @param mousePosition Position de la souris
     * @return true si la souris est sur une fenêtre
     */
    bool isMouseOverWindow(const ZappyTypes::Vector2& mousePosition) const;

private:
    /**
     * @brief Crée une fenêtre spécifique en fonction de son identifiant
     * @param id Identifiant de la fenêtre à créer
     * @param position Position de la fenêtre
     * @param dimensions Dimensions de la fenêtre
     * @param visible Visibilité initiale de la fenêtre
     */
    void createWindow(const std::string& id, const ZappyTypes::Vector2& position,
        const ZappyTypes::Vector2& dimensions, bool visible);
    /**
     * @brief Rend toutes les fenêtres régulières (non menu)
     */
    void renderRegularWindows();

    /**
     * @brief Rend la fenêtre de menu, en la créant si nécessaire
     */
    void renderMenuWindow();

    /**
     * @brief Obtient les fenêtres dans l'ordre z (avant-plan vers arrière-plan)
     * @return Vecteur des paires (identifiant, fenêtre)
     */
    std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>> getWindowsInZOrder();

    /**
     * @brief Essaie de démarrer le glissement de fenêtre dans l'ordre z
     * @param windows Liste des fenêtres ordonnées par z-order
     * @param mousePosition Position actuelle de la souris
     * @return true si une fenêtre a commencé à être déplacée
     */
    bool tryStartDraggingWindowInZOrder(
        const std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>>& windows,
        const ZappyTypes::Vector2& mousePosition);

    // Bibliothèque GUI
    std::shared_ptr<IGuiLib> m_guiLib;

    // NetworkManager pour l'envoi de commandes
    std::shared_ptr<NetworkManager> m_networkManager;

    // Map des fenêtres avec leurs identifiants
    std::unordered_map<std::string, std::shared_ptr<IUIWindow>> m_windows;

    // Données partagées
    struct {
        int x;
        int y;
        bool selected;
    } m_selectedTile;

    int m_currentViewMode;
    std::vector<std::string> m_viewModes;
};

} // namespace GUI
