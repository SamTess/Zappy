/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MenuWindow definition
*/

#pragma once

#include <memory>
#include "../AUIWindow.hpp"

namespace GUI {

class MenuWindow : public AUIWindow {
public:
    explicit MenuWindow(std::shared_ptr<IGuiLib> guiLib);
    ~MenuWindow() = default;

protected:
    void renderContent() override;
    void updateSpecificData(const GameData& gameData) override;

private:
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

    // Position initiale des fenêtres pour la réinitialisation
    ZappyTypes::Vector2 m_defaultPositions[6];

    // Méthodes de rendu pour les sous-menus
    void renderGraphicsSubmenu();
    void renderAudioSubmenu();
    void renderGameplaySubmenu();
    void renderWindowsSubmenu();
};

} // namespace GUI
