/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** MenuWindow
*/

#include <iomanip>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include "MenuWindow.hpp"

namespace GUI {

GUI::MenuWindow::MenuWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Menu"),
      _showMenu(false),
      _showAudioSubmenu(false),
      _showGameplaySubmenu(false),
      _showWindowsSubmenu(false),
      _musicVolume(0.8f),
      _sfxVolume(0.7f),
      _gameSpeed(1.0f),
      _uiTransparency(0.9f),
      _windowFactory(nullptr),
      _commandExecutor(nullptr) {
    _visible = true;
    setShowWindowBox(false);
    _defaultPositions[0] = {10, 810};   // TileInfo
    _defaultPositions[1] = {10, 500};   // PlayerInfo
    _defaultPositions[2] = {1520, 540};  // MapInfo
    _defaultPositions[3] = {1450, 10};  // TimeInfo
    _windows = {
        {"tileInfo", "Informations sur la case", 0},
        {"playerInfo", "Informations joueurs", 1},
        {"mapInfo", "Informations sur la carte", 2},
        {"timeInfo", "Informations temporelles", 3}
    };
}

void GUI::MenuWindow::drawMenuButton() {
    _guiLib->DrawPanel(_position.x, _position.y, _dimensions.x, _dimensions.y);
    _guiLib->DrawButton(_position.x, _position.y, _dimensions.x, _dimensions.y, "#185#");
}

void GUI::MenuWindow::handleMenuButtonClick(bool mouseOnButton, bool mousePressed) {
    bool menuButtonPressed = mouseOnButton && mousePressed;
    if (menuButtonPressed) {
        _showMenu = !_showMenu;
        if (!_showMenu) {
            resetAllSubmenus();
        }
    }
}

void GUI::MenuWindow::resetAllSubmenus() {
    _showAudioSubmenu = false;
    _showGameplaySubmenu = false;
    _showWindowsSubmenu = false;
}

float GUI::MenuWindow::getSubmenuHeight() const {
    if (_showAudioSubmenu)
        return 150;
    if (_showGameplaySubmenu)
        return 200;
    if (_showWindowsSubmenu)
        return 300;
    return 0;
}

void GUI::MenuWindow::handleMenuClickOutside(const ZappyTypes::Vector2& mousePosition, float menuItemWidth, float menuItemHeight, float startY, bool mousePressed) {
    if (!mousePressed)
        return;
    ZappyTypes::Rectangle menuMainRect = {_position.x, startY, menuItemWidth, menuItemHeight * 4};
    float submenuWidth = 300;
    float submenuHeight = getSubmenuHeight();
    float startX = _position.x + menuItemWidth;
    ZappyTypes::Rectangle submenuRect = {startX, startY, submenuWidth, submenuHeight};
    ZappyTypes::Rectangle buttonRect = {_position.x, _position.y, _dimensions.x, _dimensions.y};
    bool mouseOnButton = _guiLib->CheckCollisionPointRec(mousePosition, buttonRect);
    bool clickOnMainMenu = _guiLib->CheckCollisionPointRec(mousePosition, menuMainRect);
    bool clickOnSubmenu = submenuHeight > 0 && _guiLib->CheckCollisionPointRec(mousePosition, submenuRect);
    if (!clickOnMainMenu && !clickOnSubmenu && !mouseOnButton) {
        _showMenu = false;
        resetAllSubmenus();
    }
}

float GUI::MenuWindow::showSubmenu(bool submenu) {
    bool newValue = !submenu;
    if (newValue) {
        _showAudioSubmenu = false;
        _showGameplaySubmenu = false;
        _showWindowsSubmenu = false;
        if (&submenu == &_showAudioSubmenu)
            _showAudioSubmenu = true;
        else if (&submenu == &_showGameplaySubmenu)
            _showGameplaySubmenu = true;
        else if (&submenu == &_showWindowsSubmenu)
            _showWindowsSubmenu = true;
    }
    return newValue;
}

void GUI::MenuWindow::handleSubmenuButtons(float menuItemWidth, float menuItemHeight, float startY) {
    if (_guiLib->ButtonPressed(_position.x, startY + menuItemHeight, menuItemWidth, menuItemHeight, "Audio"))
        _showAudioSubmenu = showSubmenu(_showAudioSubmenu);
    if (_guiLib->ButtonPressed(_position.x, startY + menuItemHeight * 2, menuItemWidth, menuItemHeight, "Gameplay"))
        _showGameplaySubmenu = showSubmenu(_showGameplaySubmenu);
    if (_guiLib->ButtonPressed(_position.x, startY + menuItemHeight * 3, menuItemWidth, menuItemHeight, "Fenêtres"))
        _showWindowsSubmenu = showSubmenu(_showWindowsSubmenu);
}

void GUI::MenuWindow::drawMainMenu(float menuItemWidth, float menuItemHeight, float startY) {
    _guiLib->DrawPanel(_position.x, startY, menuItemWidth, menuItemHeight * 4);
}

void GUI::MenuWindow::renderContent() {
    drawMenuButton();
    ZappyTypes::Vector2 mousePosition = _guiLib->GetMousePosition();
    ZappyTypes::Rectangle buttonRect = { _position.x, _position.y, _dimensions.x, _dimensions.y};
    bool mouseOnButton = _guiLib->CheckCollisionPointRec(mousePosition, buttonRect);
    bool mousePressed = _guiLib->IsMouseButtonPressed(0);
    handleMenuButtonClick(mouseOnButton, mousePressed);
    if (_showMenu) {
        float menuItemHeight = 30;
        float menuItemWidth = _dimensions.x + 160;
        float startY = _position.y + _dimensions.y;
        drawMainMenu(menuItemWidth, menuItemHeight, startY);
        handleMenuClickOutside(mousePosition, menuItemWidth, menuItemHeight, startY, mousePressed);
        handleSubmenuButtons(menuItemWidth, menuItemHeight, startY);
        if (_showAudioSubmenu) {
            renderAudioSubmenu();
        } else if (_showGameplaySubmenu) {
            renderGameplaySubmenu();
        } else if (_showWindowsSubmenu) {
            renderWindowsSubmenu();
        }
    }
}

void GUI::MenuWindow::drawAudioSliders(float startX, float startY, float submenuWidth, float sliderHeight) {
    _guiLib->DrawLabel(startX + 30, startY + 30, submenuWidth - 60, 20, "Volume musique:");
    float newVolume = _guiLib->DrawSlider(startX + 30, startY + 50, submenuWidth - 60, sliderHeight, "0%", "100%", _musicVolume, 0.0f, 1.0f);
    if (newVolume != _musicVolume) {
        _musicVolume = newVolume;
        applyMusicVolumeChange(newVolume);
    }
}

void GUI::MenuWindow::renderAudioSubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 150;
    float sliderHeight = 30;
    float startX = _position.x + _dimensions.x + 160;
    float startY = _position.y + _dimensions.y;
    _guiLib->DrawPanel(startX, startY, submenuWidth, submenuHeight);
    _guiLib->DrawLabel(startX + 30, startY + 5, submenuWidth - 60, 20, "Paramètres audio");
    drawAudioSliders(startX, startY, submenuWidth, sliderHeight);
    bool newState = _guiLib->DrawCheckBox(startX + 30, startY + 90, 25, 25, "Activer SFX", _sfxEnabled);
    if (newState != _sfxEnabled) {
        _sfxEnabled = newState;
        applySfxEnabledChange(newState);
    }
}

void MenuWindow::drawGameplaySliders(float startX, float startY, float submenuWidth, float sliderHeight) {
    _guiLib->DrawLabel(startX + 30, startY + 30, submenuWidth - 60, 20, "Fréquence du serveur:");
    int currentFrequency = _dataProvider ? _dataProvider->getFrequency() : 7;
    std::stringstream freqSs;
    freqSs << "Actuelle: " << currentFrequency << " ticks/seconde";
    _guiLib->DrawLabel( startX + 30, startY + 50, submenuWidth - 60, 20, freqSs.str());
    static float newFrequency = static_cast<float>(currentFrequency);
    static int lastKnownFrequency = -1;
    if (lastKnownFrequency != currentFrequency) {
        newFrequency = static_cast<float>(currentFrequency);
        lastKnownFrequency = currentFrequency;
    }
    newFrequency = _guiLib->DrawSlider( startX + 30, startY + 75, submenuWidth - 60,
        sliderHeight, "1", "2000", newFrequency, 1.0f, 2000.0f);
    std::stringstream newFreqSs;
    newFreqSs << "Nouvelle: " << static_cast<int>(newFrequency) << " ticks/sec";
    _guiLib->DrawLabel( startX + 30, startY + 105, submenuWidth - 60, 20, newFreqSs.str());
    if (_guiLib->ButtonPressed( startX + 30, startY + 130, submenuWidth - 60, 25, "Appliquer fréquence")) {
        if (_commandExecutor) {
            auto frequencyCommand = std::make_shared<ServerFrequencyCommand>(static_cast<int>(newFrequency));
            _commandExecutor->executeCommand(frequencyCommand);
            currentFrequency = static_cast<int>(newFrequency);
        }
    }
}

void MenuWindow::renderGameplaySubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 200;
    float sliderHeight = 30;
    float startX = _position.x + _dimensions.x + 160;
    float startY = _position.y + _dimensions.y;
    _guiLib->DrawPanel(startX, startY, submenuWidth, submenuHeight);
    _guiLib->DrawLabel(startX + 30, startY + 5, submenuWidth - 60, 20, "Paramètres de gameplay");
    drawGameplaySliders(startX, startY, submenuWidth, sliderHeight);
}

void MenuWindow::drawWindowsList(float startX, float startY, float submenuWidth, float buttonHeight) {
    float yPos = startY + 30;
    if (!_windowFactory)
        return;
    for (const auto& window : _windows) {
        auto windowPtr = _windowFactory->getWindow(window.id);
        if (windowPtr) {
            bool isVisible = windowPtr->isVisible();
            bool newVisible = _guiLib->DrawToggle(startX + 10, yPos,
                submenuWidth - 70, buttonHeight, window.name, isVisible);
            if (newVisible != isVisible)
                windowPtr->setVisible(newVisible);
            if (_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
                if (window.positionIndex < 6)
                    windowPtr->setPosition(_defaultPositions[window.positionIndex]);
            }
            yPos += buttonHeight + 5;
        }
    }
    drawWindowButtons(startX, startY, submenuWidth, buttonHeight, yPos);
}

float MenuWindow::drawWindowButtons(float startX, float startY, float submenuWidth, float buttonHeight, float yPos) {
    (void)startY;
    if (_guiLib->ButtonPressed(startX + 10, yPos, submenuWidth - 20, buttonHeight, "Afficher toutes les fenêtres")) {
        for (const auto& window : _windows) {
            auto windowPtr = _windowFactory->getWindow(window.id);
            if (windowPtr) {
                windowPtr->setVisible(true);
            }
        }
    }
    yPos += buttonHeight + 5;
    if (_guiLib->ButtonPressed(startX + 10, yPos, submenuWidth - 20, buttonHeight, "Masquer toutes les fenêtres (sauf menu)")) {
        for (const auto& window : _windows) {
            auto windowPtr = _windowFactory->getWindow(window.id);
            if (windowPtr && window.id != "menu")
                windowPtr->setVisible(false);
        }
    }
    return yPos;
}

void MenuWindow::renderWindowsSubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 300;
    float buttonHeight = 30;
    float startX = _position.x + _dimensions.x + 160;
    float startY = _position.y + _dimensions.y;
    _guiLib->DrawPanel( startX, startY, submenuWidth, submenuHeight);
    _guiLib->DrawLabel(  startX + 30, startY + 5, submenuWidth - 60,
        20, "Gestion des fenêtres");
    drawWindowsList(startX, startY, submenuWidth, buttonHeight);
}

void MenuWindow::setUIWindowFactory(std::shared_ptr<GUI::UIWindowFactory> factory) {
    _windowFactory = factory;
    if (_windowFactory) {
        for (const auto& window : _windows) {
            auto windowPtr = _windowFactory->getWindow(window.id);
            if (windowPtr && window.positionIndex < 6) {
                windowPtr->setPosition(_defaultPositions[window.positionIndex]);
            }
        }
    }
}

void MenuWindow::setCommandExecutor(std::shared_ptr<ICommandExecutor> executor) {
    _commandExecutor = executor;
}

void MenuWindow::updateData(std::shared_ptr<IUIDataProvider> dataProvider) {
    AUIWindow::updateData(dataProvider);
    updateSettingsFromGameState();
}

void MenuWindow::updateSettingsFromGameState() {
    if (!_dataProvider)
        return;

    auto settingsProvider = std::dynamic_pointer_cast<GUI::IUISettingsProvider>(_dataProvider);
    if (settingsProvider) {
        float gameStateMusicVolume = settingsProvider->getMusicVolume();
        bool gameStateSfxEnabled = settingsProvider->getSfxEnabled();
        _musicVolume = gameStateMusicVolume;
        _sfxEnabled = gameStateSfxEnabled;
    }
}

void MenuWindow::applyMusicVolumeChange(float newVolume) {
    if (!_dataProvider)
        return;
    auto settingsProvider = std::dynamic_pointer_cast<GUI::IUISettingsProvider>(_dataProvider);
    if (settingsProvider)
        settingsProvider->setMusicVolume(newVolume);
}

void MenuWindow::applySfxEnabledChange(bool enabled) {
    std::cout << "DEBUG MenuWindow: applySfxEnabledChange(" << (enabled ? "true" : "false") << ") called" << std::endl;
    if (!_dataProvider)
        return;
    auto settingsProvider = std::dynamic_pointer_cast<GUI::IUISettingsProvider>(_dataProvider);
    if (settingsProvider) {
        settingsProvider->setSfxEnabled(enabled);
        std::cout << "DEBUG MenuWindow: SFX setting applied to GameState" << std::endl;
    } else {
        std::cout << "DEBUG MenuWindow: Failed to cast to IUISettingsProvider" << std::endl;
    }
}

} // namespace GUI
