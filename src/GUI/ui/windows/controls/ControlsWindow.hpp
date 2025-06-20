/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** ControlsWindow definition
*/

#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../AUIWindow.hpp"

namespace GUI {

class ControlsWindow : public AUIWindow {
public:
    explicit ControlsWindow(std::shared_ptr<IGuiLib> guiLib);
    ~ControlsWindow() = default;

    /**
     * @brief Définit le mode de vue actuel
     * @param mode Indice du mode de vue
     */
    void setViewMode(int mode);

    /**
     * @brief Récupère le mode de vue actuel
     * @return Indice du mode de vue
     */
    int getViewMode() const;

    /**
     * @brief Dessine le titre du mode de vue
     * @param yOffset Décalage vertical pour le dessin
     * @return Nouveau décalage vertical après le dessin
     */
    float drawViewModeTitle(float yOffset);

    /**
     * @brief Dessine les boutons pour changer le mode de vue
     * @param yOffset Décalage vertical pour le dessin
     * @return Indice du nouveau mode de vue sélectionné
     */
    int drawViewModeButtons(float yOffset);

    /**
     * @brief Dessine l'arrière-plan du bouton sélectionné
     * @param yOffset Décalage vertical pour le dessin
     * @param index Indice du bouton
     * @param buttonHeight Hauteur du bouton
     * @param spacing Espacement entre les boutons
     */
    void drawSelectedButtonBackground(float yOffset, size_t index,
        float buttonHeight, float spacing);

    /**
     *  @brief Dessine un bouton pour changer le mode de vue
     *  @param yOffset Décalage vertical pour le dessin
     *  @param index Indice du bouton
     *  @param buttonHeight Hauteur du bouton
     *  @param spacing Espacement entre les boutons
     *  @return true si le bouton a été pressé, false sinon
     */
    bool drawViewModeButton(float yOffset, size_t index,
        float buttonHeight, float spacing);

protected:
    void renderContent() override;
    void updateSpecificData(std::shared_ptr<const GameState> gameState,
                          int mapWidth, int mapHeight,
                          float gameTime, int frequency, int gameTick) override;

private:
    int m_currentViewMode;
    std::vector<std::string> m_viewModes;
};

} // namespace GUI
