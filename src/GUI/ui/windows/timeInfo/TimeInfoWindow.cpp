/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TimeInfoWindow implementation
*/

#include <iomanip>
#include <sstream>
#include <memory>
#include "TimeInfoWindow.hpp"

namespace GUI {

TimeInfoWindow::TimeInfoWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Informations temporelles") {
}

void TimeInfoWindow::renderContent() {
    float yOffset = m_position.y + 30;

    std::stringstream timeInfo;
    timeInfo << "Temps: " << std::fixed << std::setprecision(2) << m_gameTime;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        timeInfo.str()
    );
    yOffset += 25;
    std::stringstream freqInfo;
    freqInfo << "Fréquence: " << m_frequency << " Hz";
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        freqInfo.str()
    );
    yOffset += 25;
    std::stringstream tickInfo;
    tickInfo << "Tick: " << m_gameTick;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        tickInfo.str()
    );
}

void TimeInfoWindow::updateSpecificData(std::shared_ptr<const GameState> gameState,
                                        int mapWidth, int mapHeight,
                                        float gameTime, int frequency, int gameTick) {
    (void)gameState; (void)mapWidth; (void)mapHeight; (void)gameTime; (void)frequency; (void)gameTick;
    // Ces données sont maintenant directement accessible via les membres de la classe de base
}

} // namespace GUI
