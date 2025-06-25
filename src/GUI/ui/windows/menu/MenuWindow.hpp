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

namespace GUI {

class MenuWindow : public AUIWindow {
    public:
        explicit MenuWindow(std::shared_ptr<IGuiLib> guiLib);
        ~MenuWindow() = default;
        void setUIWindowFactory(std::shared_ptr<GUI::UIWindowFactory> factory);
    private:
    struct WindowInfo {
        std::string id;
        std::string name;
        int positionIndex;
    };
        bool _showMenu;
        bool _showAudioSubmenu;
        bool _showGameplaySubmenu;
        bool _showWindowsSubmenu;
        float _musicVolume;
        float _gameSpeed;
        float _uiTransparency;
        int _frequency = 100;
        std::shared_ptr<GUI::UIWindowFactory> _windowFactory;
        ZappyTypes::Vector2 _defaultPositions[6];
        std::vector<WindowInfo> _windows;
        bool _sfxEnabled;

        void renderContent() override;
        void drawMenuButton();
        void handleMenuButtonClick(bool mouseOnButton, bool mousePressed);
        void drawMainMenu(float menuItemWidth, float menuItemHeight, float startY);
        void handleMenuClickOutside(const ZappyTypes::Vector2& mousePosition, float menuItemWidth,
            float menuItemHeight, float startY, bool mousePressed);
        void handleSubmenuButtons(float menuItemWidth, float menuItemHeight, float startY);

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
