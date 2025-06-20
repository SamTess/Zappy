/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MenuWindow definition
*/

#pragma once

#include <memory>
#include <string>
#include "../AUIWindow.hpp"
#include "../../UIWindowFactory.hpp"

namespace GUI {

class MenuWindow : public AUIWindow {
public:
    explicit MenuWindow(std::shared_ptr<IGuiLib> guiLib);
    ~MenuWindow() = default;

    /**
     * @brief Définit la référence vers la factory des fenêtres
     * @param factory Pointeur vers la factory
     */
    void setUIWindowFactory(std::shared_ptr<GUI::UIWindowFactory> factory);

protected:
    void renderContent() override;
    void updateSpecificData(std::shared_ptr<const GameState> gameState,
                          int mapWidth, int mapHeight,
                          float gameTime, int frequency, int gameTick) override;

private:
    // Structure pour représenter les informations sur les fenêtres
    struct WindowInfo {
        std::string id;
        std::string name;
        int positionIndex;
    };

    // État du menu
    bool m_showMenu;
    bool m_showGraphicsSubmenu;
    bool m_showAudioSubmenu;
    bool m_showGameplaySubmenu;
    bool m_showWindowsSubmenu;

    // Paramètres du menu
    float m_zoomLevel;
    float m_musicVolume;
    float m_sfxVolume;
    float m_gameSpeed;
    float m_uiTransparency;

    // Factory pour les fenêtres
    std::shared_ptr<GUI::UIWindowFactory> m_windowFactory;
    ZappyTypes::Vector2 m_defaultPositions[6];

    // Fonctions pour le dessin du menu principal
    void drawMenuButton();
    void handleMenuButtonClick(bool mouseOnButton, bool mousePressed);
    void drawMainMenu(float menuItemWidth, float menuItemHeight, float startY);
    void handleMenuClickOutside(const ZappyTypes::Vector2& mousePosition, float menuItemWidth,
        float menuItemHeight, float startY, bool mousePressed);
    void handleSubmenuButtons(float menuItemWidth, float menuItemHeight, float startY);

    // Fonctions pour le dessin des sous-menus
    void renderGraphicsSubmenu();
    void drawGraphicsSliders(float startX, float startY, float submenuWidth, float sliderHeight);
    void renderAudioSubmenu();
    void drawAudioSliders(float startX, float startY, float submenuWidth, float sliderHeight);
    void renderGameplaySubmenu();
    void drawGameplaySliders(float startX, float startY, float submenuWidth, float sliderHeight);
    void renderWindowsSubmenu();
    void drawWindowsList(float startX, float startY, float submenuWidth, float buttonHeight);
    float drawWindowButtons(float startX, float startY, float submenuWidth, float buttonHeight, float yPos);

    // Fonctions utilitaires
    void resetAllSubmenus();
    float showSubmenu(bool submenu);
    float getSubmenuHeight() const;
};

} // namespace GUI
