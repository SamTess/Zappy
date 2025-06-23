/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** MenuWindow implementation
*/

#include <iomanip>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include "MenuWindow.hpp"
#include "../../../network/networkManager/NetworkManager.hpp"

namespace GUI {

GUI::MenuWindow::MenuWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Menu"),
      m_showMenu(false),
      m_showGraphicsSubmenu(false),
      m_showAudioSubmenu(false),
      m_showGameplaySubmenu(false),
      m_showWindowsSubmenu(false),
      m_zoomLevel(1.0f),
      m_musicVolume(0.8f),
      m_sfxVolume(0.7f),
      m_gameSpeed(1.0f),
      m_uiTransparency(0.9f),
      m_windowFactory(nullptr) {
    m_visible = true;
    m_defaultPositions[0] = {10, 810};   // TileInfo
    m_defaultPositions[1] = {10, 500};   // PlayerInfo
    m_defaultPositions[2] = {1520, 540};  // Broadcasts
    m_defaultPositions[3] = {1520, 860};  // Controls
    m_defaultPositions[4] = {1450, 10};  // TimeInfo

    // Initialisation de la liste unifiée des fenêtres
    m_windows = {
        {"tileInfo", "Informations sur la case", 0},
        {"playerInfo", "Informations joueurs", 1},
        {"broadcasts", "Broadcasts récents", 2},
        {"controls", "Contrôles", 3},
        {"timeInfo", "Informations temporelles", 4}
    };
}

void GUI::MenuWindow::drawMenuButton() {
    m_guiLib->DrawPanel(m_position.x, m_position.y, m_dimensions.x, m_dimensions.y);
    m_guiLib->DrawButton(
        m_position.x, m_position.y,
        m_dimensions.x, m_dimensions.y,
        "#185#"
    );
}

void GUI::MenuWindow::handleMenuButtonClick(bool mouseOnButton, bool mousePressed) {
    bool menuButtonPressed = mouseOnButton && mousePressed;
    if (menuButtonPressed) {
        m_showMenu = !m_showMenu;
        if (!m_showMenu) {
            resetAllSubmenus();
        }
    }
}

void GUI::MenuWindow::resetAllSubmenus() {
    m_showGraphicsSubmenu = false;
    m_showAudioSubmenu = false;
    m_showGameplaySubmenu = false;
    m_showWindowsSubmenu = false;
}

float GUI::MenuWindow::getSubmenuHeight() const {
    if (m_showGraphicsSubmenu) return 150;
    if (m_showAudioSubmenu) return 150;
    if (m_showGameplaySubmenu) return 200;
    if (m_showWindowsSubmenu) return 300;
    return 0;
}

void GUI::MenuWindow::handleMenuClickOutside(const ZappyTypes::Vector2& mousePosition, float menuItemWidth, float menuItemHeight, float startY, bool mousePressed) {
    if (!mousePressed) return;
    ZappyTypes::Rectangle menuMainRect = {
        m_position.x, startY,
        menuItemWidth, menuItemHeight * 4
    };
    float submenuWidth = 300;
    float submenuHeight = getSubmenuHeight();
    float startX = m_position.x + menuItemWidth;
    ZappyTypes::Rectangle submenuRect = {
        startX, startY,
        submenuWidth, submenuHeight
    };
    ZappyTypes::Rectangle buttonRect = {
        m_position.x, m_position.y,
        m_dimensions.x, m_dimensions.y
    };
    bool mouseOnButton = m_guiLib->CheckCollisionPointRec(mousePosition, buttonRect);
    bool clickOnMainMenu = m_guiLib->CheckCollisionPointRec(mousePosition, menuMainRect);
    bool clickOnSubmenu = submenuHeight > 0 && m_guiLib->CheckCollisionPointRec(mousePosition, submenuRect);
    if (!clickOnMainMenu && !clickOnSubmenu && !mouseOnButton) {
        m_showMenu = false;
        resetAllSubmenus();
    }
}

float GUI::MenuWindow::showSubmenu(bool submenu) {
    bool newValue = !submenu;
    if (newValue) {
        m_showGraphicsSubmenu = false;
        m_showAudioSubmenu = false;
        m_showGameplaySubmenu = false;
        m_showWindowsSubmenu = false;
        if (&submenu == &m_showGraphicsSubmenu) m_showGraphicsSubmenu = true;
        else if (&submenu == &m_showAudioSubmenu) m_showAudioSubmenu = true;
        else if (&submenu == &m_showGameplaySubmenu) m_showGameplaySubmenu = true;
        else if (&submenu == &m_showWindowsSubmenu) m_showWindowsSubmenu = true;
    }
    return newValue;
}

void GUI::MenuWindow::handleSubmenuButtons(float menuItemWidth, float menuItemHeight, float startY) {
    if (m_guiLib->ButtonPressed(
        m_position.x, startY,
        menuItemWidth, menuItemHeight,
        "Graphiques"
    )) {
        m_showGraphicsSubmenu = showSubmenu(m_showGraphicsSubmenu);
    }
    if (m_guiLib->ButtonPressed(
        m_position.x, startY + menuItemHeight,
        menuItemWidth, menuItemHeight,
        "Audio"
    )) {
        m_showAudioSubmenu = showSubmenu(m_showAudioSubmenu);
    }
    if (m_guiLib->ButtonPressed(
        m_position.x, startY + menuItemHeight * 2,
        menuItemWidth, menuItemHeight,
        "Gameplay"
    )) {
        m_showGameplaySubmenu = showSubmenu(m_showGameplaySubmenu);
    }
    if (m_guiLib->ButtonPressed(
        m_position.x, startY + menuItemHeight * 3,
        menuItemWidth, menuItemHeight,
        "Fenêtres"
    )) {
        m_showWindowsSubmenu = showSubmenu(m_showWindowsSubmenu);
    }
}

void GUI::MenuWindow::drawMainMenu(float menuItemWidth, float menuItemHeight, float startY) {
    m_guiLib->DrawPanel(
        m_position.x,
        startY,
        menuItemWidth,
        menuItemHeight * 4
    );
}

void GUI::MenuWindow::renderContent() {
    drawMenuButton();
    ZappyTypes::Vector2 mousePosition = m_guiLib->GetMousePosition();
    ZappyTypes::Rectangle buttonRect = {
        m_position.x, m_position.y,
        m_dimensions.x, m_dimensions.y
    };
    bool mouseOnButton = m_guiLib->CheckCollisionPointRec(mousePosition, buttonRect);
    bool mousePressed = m_guiLib->IsMouseButtonPressed(0);
    handleMenuButtonClick(mouseOnButton, mousePressed);
    if (m_showMenu) {
        float menuItemHeight = 30;
        float menuItemWidth = m_dimensions.x + 160;
        float startY = m_position.y + m_dimensions.y;
        drawMainMenu(menuItemWidth, menuItemHeight, startY);
        handleMenuClickOutside(mousePosition, menuItemWidth, menuItemHeight, startY, mousePressed);
        handleSubmenuButtons(menuItemWidth, menuItemHeight, startY);
        if (m_showGraphicsSubmenu) {
            renderGraphicsSubmenu();
        } else if (m_showAudioSubmenu) {
            renderAudioSubmenu();
        } else if (m_showGameplaySubmenu) {
            renderGameplaySubmenu();
        } else if (m_showWindowsSubmenu) {
            renderWindowsSubmenu();
        }
    }
}

void GUI::MenuWindow::drawGraphicsSliders(float startX, float startY, float submenuWidth, float sliderHeight) {
    m_guiLib->DrawLabel(
        startX + 30, startY + 30,
        submenuWidth - 60, 20,
        "Niveau de zoom:"
    );
    m_zoomLevel = m_guiLib->DrawSlider(
        startX + 30, startY + 50,
        submenuWidth - 60, sliderHeight,
        "0.5x", "2.0x",
        m_zoomLevel, 0.5f, 2.0f
    );
    m_guiLib->DrawLabel(
        startX + 30, startY + 80,
        submenuWidth - 60, 20,
        "Transparence UI:"
    );
    m_uiTransparency = m_guiLib->DrawSlider(
        startX + 30, startY + 100,
        submenuWidth - 60, sliderHeight,
        "0%", "100%",
        m_uiTransparency, 0.0f, 1.0f
    );
}

void GUI::MenuWindow::renderGraphicsSubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 150;
    float sliderHeight = 30;
    float startX = m_position.x + m_dimensions.x + 160;
    float startY = m_position.y + m_dimensions.y;
    m_guiLib->DrawPanel(
        startX, startY,
        submenuWidth, submenuHeight
    );
    m_guiLib->DrawLabel(
        startX + 30, startY + 5,
        submenuWidth - 60, 20,
        "Paramètres graphiques"
    );
    drawGraphicsSliders(startX, startY, submenuWidth, sliderHeight);
}

void GUI::MenuWindow::drawAudioSliders(float startX, float startY, float submenuWidth, float sliderHeight) {
    m_guiLib->DrawLabel(
        startX + 30, startY + 30,
        submenuWidth - 60, 20,
        "Volume musique:"
    );
    m_musicVolume = m_guiLib->DrawSlider(
        startX + 30, startY + 50,
        submenuWidth - 60, sliderHeight,
        "0%", "100%",
        m_musicVolume, 0.0f, 1.0f
    );
    m_guiLib->DrawLabel(
        startX + 30, startY + 80,
        submenuWidth - 60, 20,
        "Volume SFX:"
    );
    m_sfxVolume = m_guiLib->DrawSlider(
        startX + 30, startY + 100,
        submenuWidth - 60, sliderHeight,
        "0%", "100%",
        m_sfxVolume, 0.0f, 1.0f
    );
}

void GUI::MenuWindow::renderAudioSubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 150;
    float sliderHeight = 30;
    float startX = m_position.x + m_dimensions.x + 160;
    float startY = m_position.y + m_dimensions.y;
    m_guiLib->DrawPanel(
        startX, startY,
        submenuWidth, submenuHeight
    );
    m_guiLib->DrawLabel(
        startX + 30, startY + 5,
        submenuWidth - 60, 20,
        "Paramètres audio"
    );
    drawAudioSliders(startX, startY, submenuWidth, sliderHeight);
}

void MenuWindow::drawGameplaySliders(float startX, float startY, float submenuWidth, float sliderHeight) {
    m_guiLib->DrawLabel(
        startX + 30, startY + 30,
        submenuWidth - 60, 20,
        "Fréquence du serveur:"
    );
    int currentFrequency = m_frequency;
    std::stringstream freqSs;
    freqSs << "Actuelle: " << currentFrequency << " ticks/seconde";
    m_guiLib->DrawLabel(
        startX + 30, startY + 50,
        submenuWidth - 60, 20,
        freqSs.str()
    );
    static float newFrequency = static_cast<float>(currentFrequency);
    newFrequency = m_guiLib->DrawSlider(
        startX + 30, startY + 75,
        submenuWidth - 60, sliderHeight,
        "1", "2000",
        newFrequency, 1.0f, 2000.0f
    );
    std::stringstream newFreqSs;
    newFreqSs << "Nouvelle: " << static_cast<int>(newFrequency) << " ticks/sec";
    m_guiLib->DrawLabel(
        startX + 30, startY + 105,
        submenuWidth - 60, 20,
        newFreqSs.str()
    );
    if (m_guiLib->ButtonPressed(
        startX + 30, startY + 130,
        submenuWidth - 60, 25,
        "Appliquer fréquence"
    )) {
        if (m_windowFactory && m_windowFactory->getNetworkManager()) {
            auto networkManager = m_windowFactory->getNetworkManager();
            std::stringstream cmd;
            cmd << "sst " << static_cast<int>(newFrequency);
            networkManager->sendCommand(cmd.str());
        }
    }
}

void MenuWindow::renderGameplaySubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 200;
    float sliderHeight = 30;
    float startX = m_position.x + m_dimensions.x + 160;
    float startY = m_position.y + m_dimensions.y;
    m_guiLib->DrawPanel(
        startX, startY,
        submenuWidth, submenuHeight
    );
    m_guiLib->DrawLabel(
        startX + 30, startY + 5,
        submenuWidth - 60, 20,
        "Paramètres de gameplay"
    );
    drawGameplaySliders(startX, startY, submenuWidth, sliderHeight);
}

void MenuWindow::drawWindowsList(float startX, float startY, float submenuWidth, float buttonHeight) {
    float yPos = startY + 30;
    if (!m_windowFactory) {
        m_guiLib->DrawLabel(
            startX + 10, yPos,
            submenuWidth - 20, buttonHeight * 2,
            "La factory de fenêtres n'est pas disponible.\nVeuillez l'initialiser d'abord."
        );
        return;
    }
    for (const auto& window : m_windows) {
        auto windowPtr = m_windowFactory->getWindow(window.id);
        if (windowPtr) {
            bool isVisible = windowPtr->isVisible();
            bool newVisible = m_guiLib->DrawToggle(
                startX + 10, yPos,
                submenuWidth - 70, buttonHeight,
                window.name,
                isVisible
            );
            if (newVisible != isVisible) {
                windowPtr->setVisible(newVisible);
            }
            if (m_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
                if (window.positionIndex < 6) {
                    windowPtr->setPosition(m_defaultPositions[window.positionIndex]);
                }
            }
            yPos += buttonHeight + 5;
        }
    }
    drawWindowButtons(startX, startY, submenuWidth, buttonHeight, yPos);
}

float MenuWindow::drawWindowButtons(float startX, float startY, float submenuWidth, float buttonHeight, float yPos) {
    (void)startY;
    if (m_guiLib->ButtonPressed(startX + 10, yPos, submenuWidth - 20, buttonHeight, "Afficher toutes les fenêtres")) {
        for (const auto& window : m_windows) {
            auto windowPtr = m_windowFactory->getWindow(window.id);
            if (windowPtr) {
                windowPtr->setVisible(true);
            }
        }
    }
    yPos += buttonHeight + 5;
    if (m_guiLib->ButtonPressed(startX + 10, yPos, submenuWidth - 20, buttonHeight, "Masquer toutes les fenêtres (sauf menu)")) {
        for (const auto& window : m_windows) {
            auto windowPtr = m_windowFactory->getWindow(window.id);
            if (windowPtr && window.id != "menu") {
                windowPtr->setVisible(false);
            }
        }
    }
    return yPos;
}

void MenuWindow::renderWindowsSubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 300;
    float buttonHeight = 30;
    float startX = m_position.x + m_dimensions.x + 160;
    float startY = m_position.y + m_dimensions.y;
    m_guiLib->DrawPanel(
        startX, startY,
        submenuWidth, submenuHeight
    );
    m_guiLib->DrawLabel(
        startX + 30, startY + 5,
        submenuWidth - 60, 20,
        "Gestion des fenêtres"
    );
    drawWindowsList(startX, startY, submenuWidth, buttonHeight);
}

void MenuWindow::setUIWindowFactory(std::shared_ptr<GUI::UIWindowFactory> factory) {
    m_windowFactory = factory;
    if (m_windowFactory) {
        for (const auto& window : m_windows) {
            auto windowPtr = m_windowFactory->getWindow(window.id);
            if (windowPtr && window.positionIndex < 6) {
                windowPtr->setPosition(m_defaultPositions[window.positionIndex]);
            }
        }
    }
}

} // namespace GUI
