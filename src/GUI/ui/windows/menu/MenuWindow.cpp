/*
** EPITECH PROJECT, 2025
*    m_visible = true;
    m_defaultPositions[0] = {10, 810};   // TileInfo
    m_defaultPositions[1] = {10, 500};   // PlayerInfo
    m_defaultPositions[2] = {1520, 540};  // Broadcasts
    m_defaultPositions[3] = {1520, 860};  // Controls
    m_defaultPositions[4] = {1450, 10};   // TimeInfo-400
** File description:
** MenuWindow implementation
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
}

void GUI::MenuWindow::renderContent() {
    m_guiLib->DrawPanel(m_position.x, m_position.y, m_dimensions.x, m_dimensions.y);
    m_guiLib->DrawButton(
        m_position.x, m_position.y,
        m_dimensions.x, m_dimensions.y,
        "#185#"
    );
    ZappyTypes::Vector2 mousePosition = m_guiLib->GetMousePosition();
    ZappyTypes::Rectangle buttonRect = {
        m_position.x, m_position.y,
        m_dimensions.x, m_dimensions.y
    };
    bool mouseOnButton = m_guiLib->CheckCollisionPointRec(mousePosition, buttonRect);
    bool mousePressed = m_guiLib->IsMouseButtonPressed(0);
    bool menuButtonPressed = mouseOnButton && mousePressed;
    if (menuButtonPressed) {
        m_showMenu = !m_showMenu;
        if (!m_showMenu) {
            m_showGraphicsSubmenu = false;
            m_showAudioSubmenu = false;
            m_showGameplaySubmenu = false;
            m_showWindowsSubmenu = false;
        }
    }
    if (m_showMenu) {
        float menuItemHeight = 30;
        float menuItemWidth = m_dimensions.x + 160;
        float startY = m_position.y + m_dimensions.y;
        m_guiLib->DrawPanel(
            m_position.x,
            startY,
            menuItemWidth,
            menuItemHeight * 4
        );
        if (mousePressed) {
            ZappyTypes::Rectangle menuMainRect = {
                m_position.x, startY,
                menuItemWidth, menuItemHeight * 4
            };
            float submenuWidth = 300;
            float submenuHeight = m_showGraphicsSubmenu ? 150 :
                                (m_showAudioSubmenu ? 150 :
                                (m_showGameplaySubmenu ? 150 :
                                (m_showWindowsSubmenu ? 300 : 0)));
            float startX = m_position.x + menuItemWidth;
            ZappyTypes::Rectangle submenuRect = {
                startX, startY,
                submenuWidth, submenuHeight
            };
            bool clickOnMainMenu = m_guiLib->CheckCollisionPointRec(mousePosition, menuMainRect);
            bool clickOnSubmenu = submenuHeight > 0 && m_guiLib->CheckCollisionPointRec(mousePosition, submenuRect);
            if (!clickOnMainMenu && !clickOnSubmenu && !mouseOnButton) {
                m_showMenu = false;
                m_showGraphicsSubmenu = false;
                m_showAudioSubmenu = false;
                m_showGameplaySubmenu = false;
                m_showWindowsSubmenu = false;
            }
        }
        if (m_guiLib->ButtonPressed(
            m_position.x, startY,
            menuItemWidth, menuItemHeight,
            "Graphiques"
        )) {
            m_showGraphicsSubmenu = !m_showGraphicsSubmenu;
            m_showAudioSubmenu = false;
            m_showGameplaySubmenu = false;
            m_showWindowsSubmenu = false;
        }
        if (m_guiLib->ButtonPressed(
            m_position.x, startY + menuItemHeight,
            menuItemWidth, menuItemHeight,
            "Audio"
        )) {
            m_showAudioSubmenu = !m_showAudioSubmenu;
            m_showGraphicsSubmenu = false;
            m_showGameplaySubmenu = false;
            m_showWindowsSubmenu = false;
        }
        if (m_guiLib->ButtonPressed(
            m_position.x, startY + menuItemHeight * 2,
            menuItemWidth, menuItemHeight,
            "Gameplay"
        )) {
            m_showGameplaySubmenu = !m_showGameplaySubmenu;
            m_showGraphicsSubmenu = false;
            m_showAudioSubmenu = false;
            m_showWindowsSubmenu = false;
        }
        if (m_guiLib->ButtonPressed(
            m_position.x, startY + menuItemHeight * 3,
            menuItemWidth, menuItemHeight,
            "Fenêtres"
        )) {
            m_showWindowsSubmenu = !m_showWindowsSubmenu;
            m_showGraphicsSubmenu = false;
            m_showAudioSubmenu = false;
            m_showGameplaySubmenu = false;
        }
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

void MenuWindow::renderGameplaySubmenu() {
    float submenuWidth = 300;
    float submenuHeight = 120;
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
    m_guiLib->DrawLabel(
        startX + 30, startY + 30,
        submenuWidth - 60, 20,
        "Vitesse du jeu:"
    );
    m_gameSpeed = m_guiLib->DrawSlider(
        startX + 30, startY + 50,
        submenuWidth - 60, sliderHeight,
        "0.1x", "5.0x",
        m_gameSpeed, 0.1f, 5.0f
    );
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << m_gameSpeed << "x";
    m_guiLib->DrawLabel(
        startX + 30, startY + 80,
        submenuWidth - 60, 20,
        "Valeur actuelle: " + ss.str()
    );
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
    float yPos = startY + 30;
    if (!m_windowFactory) {
        m_guiLib->DrawLabel(
            startX + 10, yPos,
            submenuWidth - 20, buttonHeight * 2,
            "La factory de fenêtres n'est pas disponible.\nVeuillez l'initialiser d'abord."
        );
        return;
    }
    struct WindowInfo {
        std::string id;
        std::string name;
        int positionIndex;
    };

    WindowInfo windows[] = {
        {"tileInfo", "Informations sur la case", 0},
        {"playerInfo", "Informations joueurs", 1},
        {"broadcasts", "Broadcasts récents", 2},
        {"controls", "Contrôles", 3},
        {"timeInfo", "Informations temporelles", 4}
    };

    for (const auto& window : windows) {
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
    if (m_guiLib->ButtonPressed(startX + 10, yPos, submenuWidth - 20, buttonHeight, "Afficher toutes les fenêtres")) {
        for (const auto& window : windows) {
            auto windowPtr = m_windowFactory->getWindow(window.id);
            if (windowPtr) {
                windowPtr->setVisible(true);
            }
        }
    }
    yPos += buttonHeight + 5;
    if (m_guiLib->ButtonPressed(startX + 10, yPos, submenuWidth - 20, buttonHeight, "Masquer toutes les fenêtres (sauf menu)")) {
        for (const auto& window : windows) {
            auto windowPtr = m_windowFactory->getWindow(window.id);
            if (windowPtr && window.id != "menu") {
                windowPtr->setVisible(false);
            }
        }
    }
}

void GUI::MenuWindow::updateSpecificData(const GameData& gameData) {
    (void)gameData;
}

void MenuWindow::setUIWindowFactory(std::shared_ptr<GUI::UIWindowFactory> factory) {
    m_windowFactory = factory;
    if (m_windowFactory) {
        for (int i = 0; i < 6; ++i) {
            m_windowFactory->getWindow("tileInfo")->setPosition(m_defaultPositions[0]);
            m_windowFactory->getWindow("playerInfo")->setPosition(m_defaultPositions[1]);
            m_windowFactory->getWindow("broadcasts")->setPosition(m_defaultPositions[2]);
            m_windowFactory->getWindow("controls")->setPosition(m_defaultPositions[3]);
            m_windowFactory->getWindow("timeInfo")->setPosition(m_defaultPositions[4]);
        }
    }
}

} // namespace GUI
