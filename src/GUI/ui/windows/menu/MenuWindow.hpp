/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MenuWindow definition
*/

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../AUIWindow.hpp"
#include "../../UIWindowFactory.hpp"
#include "../../../shared/commands/ICommand.hpp"
#include "../../../shared/commands/ICommandExecutor.hpp"
#include "ServerFrequencyCommand.hpp"

namespace GUI {

class MenuWindow : public AUIWindow {
public:
    explicit MenuWindow(std::shared_ptr<IGuiLib> guiLib);
    ~MenuWindow() = default;

    void setUIWindowFactory(std::shared_ptr<GUI::UIWindowFactory> factory);
    void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor);

protected:
    void renderContent() override;

private:
    struct WindowInfo {
        std::string id;
        std::string name;
        int positionIndex;
    };
    bool m_showMenu;
    bool m_showGraphicsSubmenu;
    bool m_showAudioSubmenu;
    bool m_showGameplaySubmenu;
    bool m_showWindowsSubmenu;
    float m_zoomLevel;
    float m_musicVolume;
    float m_sfxVolume;
    float m_gameSpeed;
    float m_uiTransparency;

    std::shared_ptr<GUI::UIWindowFactory> m_windowFactory;
    std::shared_ptr<ICommandExecutor> m_commandExecutor;
    ZappyTypes::Vector2 m_defaultPositions[6];
    std::vector<WindowInfo> m_windows;
    void drawMenuButton();
    void handleMenuButtonClick(bool mouseOnButton, bool mousePressed);
    void drawMainMenu(float menuItemWidth, float menuItemHeight, float startY);
    void handleMenuClickOutside(const ZappyTypes::Vector2& mousePosition, float menuItemWidth,
        float menuItemHeight, float startY, bool mousePressed);
    void handleSubmenuButtons(float menuItemWidth, float menuItemHeight, float startY);
    void renderGraphicsSubmenu();
    void drawGraphicsSliders(float startX, float startY, float submenuWidth, float sliderHeight);
    void renderAudioSubmenu();
    void drawAudioSliders(float startX, float startY, float submenuWidth, float sliderHeight);
    void renderGameplaySubmenu();
    void drawGameplaySliders(float startX, float startY, float submenuWidth, float sliderHeight);
    void renderWindowsSubmenu();
    void drawWindowsList(float startX, float startY, float submenuWidth, float buttonHeight);
    float drawWindowButtons(float startX, float startY, float submenuWidth, float buttonHeight, float yPos);
    void resetAllSubmenus();
    float showSubmenu(bool submenu);
    float getSubmenuHeight() const;
};

} // namespace GUI
