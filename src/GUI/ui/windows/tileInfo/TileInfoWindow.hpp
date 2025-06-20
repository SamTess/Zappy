/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TileInfoWindow implementation
*/

#pragma once

#include <memory>
#include <utility>
#include "../AUIWindow.hpp"

namespace GUI {

class TileInfoWindow : public AUIWindow {
public:
    explicit TileInfoWindow(std::shared_ptr<IGuiLib> guiLib);
    ~TileInfoWindow() = default;

    /**
     * @brief Met à jour la tuile sélectionnée
     * @param x Coordonnée X de la tuile
     * @param y Coordonnée Y de la tuile
     */
    void setSelectedTile(int x, int y);
    /**
     * @brief Récupère les coordonnées de la tuile sélectionnée
     * @return Paire de coordonnées (x, y)
     */
    std::pair<int, int> getSelectedTile() const;
    /**
     * @brief Vérifie si une tuile est sélectionnée
     * @return true si une tuile est sélectionnée, false sinon
     */
    bool hasTileSelected() const;

protected:
    void renderContent() override;

private:
    /**
     * @brief Affiche un message quand aucune tuile n'est sélectionnée
     */
    void renderNoTileSelected();

    /**
     * @brief Affiche les coordonnées de la tuile sélectionnée
     */
    void renderTilePosition();

    /**
     * @brief Affiche les informations sur les ressources de la tuile
     * @return Position Y pour le prochain élément à afficher
     */
    float renderResourceInfo();

    /**
     * @brief Affiche le nombre de joueurs sur la tuile
     * @param yOffset Position Y où commencer l'affichage
     */
    void renderPlayerCount(float yOffset);

    struct {
        int x;
        int y;
        bool selected;
    } m_selectedTile;
};

} // namespace GUI
