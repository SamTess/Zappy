/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MenuWindow
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
typedef struct WindowInfo_s {
    std::string id;
    std::string name;
    int positionIndex;
} WindowInfo_t;

class MenuWindow : public AUIWindow {
    public:
        explicit MenuWindow(std::shared_ptr<IGuiLib> guiLib);
        ~MenuWindow() = default;

        void setUIWindowFactory(std::shared_ptr<GUI::UIWindowFactory> factory);
        void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor);
        void updateData(std::shared_ptr<IUIDataProvider> dataProvider) override;

    protected:
        void renderContent() override;

    private:
        bool _showMenu;
        bool _showAudioSubmenu;
        bool _showGameplaySubmenu;
        bool _showWindowsSubmenu;
        float _musicVolume;
        float _sfxVolume;
        float _gameSpeed;
        float _uiTransparency;
        int _frequency = 100;
        bool _sfxEnabled = true;
        std::shared_ptr<GUI::UIWindowFactory> _windowFactory;
        std::shared_ptr<ICommandExecutor> _commandExecutor;
        ZappyTypes::Vector2 _defaultPositions[6];
        std::vector<WindowInfo_t> _windows;

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
        void updateSettingsFromGameState();
        void applyMusicVolumeChange(float newVolume);
        void applySfxEnabledChange(bool enabled);
};

} // namespace GUI
