/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** MenuWindow implementation
*/

#include <iomanip>
#include <memory>
#include <iostream>
#include <sstream>
#include "MenuWindow.hpp"

namespace GUI {

MenuWindow::MenuWindow(std::shared_ptr<IGuiLib> guiLib)
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
      m_uiTransparency(0.9f) {
    m_visible = true;
    m_defaultPositions[0] = {20, 40};   // Logs
    m_defaultPositions[1] = {50, 50};   // TileInfo
    m_defaultPositions[2] = {80, 60};   // PlayerInfo
    m_defaultPositions[3] = {110, 70};  // Broadcasts
    m_defaultPositions[4] = {140, 80};  // Controls
    m_defaultPositions[5] = {170, 90};  // TimeInfo
}

void MenuWindow::renderContent() {
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

void MenuWindow::renderGraphicsSubmenu() {
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

void MenuWindow::renderAudioSubmenu() {
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
    // Référence vers les fenêtres externes via les onglets de l'UI
    // Cette implémentation serait à adapter pour être liée à la factory
    float yPos = startY + 30;
    // TODO(Sam): Adapter cette partie pour utiliser la factory
    // Ci-dessous, le code est une simplification qui devrait être remplacée
    // par des appels à la factory pour récupérer et modifier les fenêtres

    // Exemple pour les logs
    bool showLogs = true;
    showLogs = m_guiLib->DrawToggle(
        startX + 10, yPos,
        submenuWidth - 20, buttonHeight,
        "Journal d'événements",
        showLogs
    );
    if (m_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
        // Réinitialiser la position
    }
    yPos += buttonHeight + 5;
    bool showTileInfo = true;
    showTileInfo = m_guiLib->DrawToggle(
        startX + 10, yPos,
        submenuWidth - 20, buttonHeight,
        "Informations sur la case",
        showTileInfo
    );
    if (m_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
        // Réinitialiser la position
    }
    yPos += buttonHeight + 5;
    bool showPlayerInfo = true;
    showPlayerInfo = m_guiLib->DrawToggle(
        startX + 10, yPos,
        submenuWidth - 20, buttonHeight,
        "Informations joueurs",
        showPlayerInfo
    );
    if (m_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
        // Réinitialiser la position
    }
    yPos += buttonHeight + 5;
    // Broadcasts
    bool showBroadcasts = true;
    showBroadcasts = m_guiLib->DrawToggle(
        startX + 10, yPos,
        submenuWidth - 20, buttonHeight,
        "Broadcasts récents",
        showBroadcasts
    );
    if (m_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
        // Réinitialiser la position
    }
    yPos += buttonHeight + 5;

    // Controls
    bool showControls = true;
    showControls = m_guiLib->DrawToggle(
        startX + 10, yPos,
        submenuWidth - 20, buttonHeight,
        "Contrôles",
        showControls
    );
    if (m_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
        // Réinitialiser la position
    }
    yPos += buttonHeight + 5;

    // Time Info
    bool showTimeInfo = true;
    showTimeInfo = m_guiLib->DrawToggle(
        startX + 10, yPos,
        submenuWidth - 20, buttonHeight,
        "Informations temporelles",
        showTimeInfo
    );
    if (m_guiLib->ButtonPressed(startX + submenuWidth - 60, yPos, 50, buttonHeight, "Reset")) {
        // Réinitialiser la position
    }
}

void MenuWindow::updateSpecificData(const GameData& gameData) {
    // Rien à mettre à jour pour le menu à partir des données de jeu
    (void)gameData;
}

} // namespace GUI
