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
      m_showLogs(true),
      m_showTileInfo(true),
      m_showPlayerInfo(true),
      m_showBroadcasts(true),
      m_showTimeInfo(true),
      m_currentViewMode(0) {
    m_viewModes = {"Vue standard", "Vue aérienne", "Vue wireframe", "Vue ressources"};
}

void ControlsWindow::renderContent() {
    float yOffset = m_position.y + 30;
    m_showLogs = m_guiLib->DrawToggle(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        25,
        "Afficher les logs",
        m_showLogs
    );
    yOffset += 30;
    m_showTileInfo = m_guiLib->DrawToggle(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        25,
        "Informations de case",
        m_showTileInfo
    );
    yOffset += 30;
    m_showPlayerInfo = m_guiLib->DrawToggle(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        25,
        "Informations joueurs",
        m_showPlayerInfo
    );
    yOffset += 30;
    m_showBroadcasts = m_guiLib->DrawToggle(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        25,
        "Broadcasts récents",
        m_showBroadcasts
    );
    yOffset += 30;
    m_showTimeInfo = m_guiLib->DrawToggle(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        25,
        "Informations temporelles",
        m_showTimeInfo
    );
    yOffset += 30;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        "Mode de vue:"
    );
    yOffset += 25;

    float buttonHeight = 25;
    float spacing = 5;
    int newSelectedMode = m_currentViewMode;

    for (size_t i = 0; i < m_viewModes.size(); i++) {
        bool isSelected = (static_cast<int>(i) == m_currentViewMode);
        if (isSelected) {
            m_guiLib->DrawPanel(
                m_position.x + 8,
                yOffset + i * (buttonHeight + spacing) - 2,
                m_dimensions.x - 16,
                buttonHeight + 4
            );
        }
        if (m_guiLib->ButtonPressed(
                m_position.x + 10,
                yOffset + i * (buttonHeight + spacing),
                m_dimensions.x - 20,
                buttonHeight,
                m_viewModes[i])) {
            newSelectedMode = i;
        }
    }
    if (newSelectedMode != m_currentViewMode)
        setViewMode(newSelectedMode);
}

void ControlsWindow::updateSpecificData(const GameData& gameData) {
    (void)gameData;
}

void ControlsWindow::setViewMode(int mode) {
    if (mode >= 0 && mode < static_cast<int>(m_viewModes.size()))
        m_currentViewMode = mode;
}

int ControlsWindow::getViewMode() const {
    return m_currentViewMode;
}

} // namespace GUI
