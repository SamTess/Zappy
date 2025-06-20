/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** ControlsWindow implementation
*/

#include "ControlsWindow.hpp"
#include <memory>

namespace GUI {

ControlsWindow::ControlsWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Contrôles"),
      m_currentViewMode(0) {
    m_viewModes = {"Vue standard", "Vue aérienne", "Vue wireframe", "Vue ressources"};
}

void ControlsWindow::renderContent() {
    float yOffset = m_position.y + 30;
    yOffset = drawViewModeTitle(yOffset);
    int newSelectedMode = drawViewModeButtons(yOffset);
    if (newSelectedMode != m_currentViewMode)
        setViewMode(newSelectedMode);
}

float ControlsWindow::drawViewModeTitle(float yOffset) {
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        "Mode de vue:"
    );
    return yOffset + 25;
}

int ControlsWindow::drawViewModeButtons(float yOffset) {
    float buttonHeight = 25;
    float spacing = 5;
    int newSelectedMode = m_currentViewMode;

    for (size_t i = 0; i < m_viewModes.size(); i++) {
        bool isSelected = (static_cast<int>(i) == m_currentViewMode);
        if (isSelected)
            drawSelectedButtonBackground(yOffset, i, buttonHeight, spacing);
        if (drawViewModeButton(yOffset, i, buttonHeight, spacing))
            newSelectedMode = i;
    }
    return newSelectedMode;
}

void ControlsWindow::drawSelectedButtonBackground(float yOffset, size_t index,
    float buttonHeight, float spacing) {
    m_guiLib->DrawPanel(
        m_position.x + 8,
        yOffset + index * (buttonHeight + spacing) - 2,
        m_dimensions.x - 16,
        buttonHeight + 4
    );
}

bool ControlsWindow::drawViewModeButton(float yOffset, size_t index,
    float buttonHeight, float spacing) {
    return m_guiLib->ButtonPressed(
        m_position.x + 10,
        yOffset + index * (buttonHeight + spacing),
        m_dimensions.x - 20,
        buttonHeight,
        m_viewModes[index]
    );
}

void ControlsWindow::updateSpecificData(std::shared_ptr<const GameState> gameState,
                                        int mapWidth, int mapHeight,
                                        float gameTime, int frequency, int gameTick) {
    (void)gameState; (void)mapWidth; (void)mapHeight; (void)gameTime; (void)frequency; (void)gameTick;
    // Cette fenêtre n'a pas besoin de données spécifiques du jeu
}

void ControlsWindow::setViewMode(int mode) {
    if (mode >= 0 && mode < static_cast<int>(m_viewModes.size()))
        m_currentViewMode = mode;
}

int ControlsWindow::getViewMode() const {
    return m_currentViewMode;
}

} // namespace GUI
