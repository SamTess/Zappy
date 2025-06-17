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
    timeInfo << "Temps: " << std::fixed << std::setprecision(2) << m_gameData.time;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        timeInfo.str()
    );
    yOffset += 25;
    std::stringstream freqInfo;
    freqInfo << "Fréquence: " << m_gameData.frequency << " Hz";
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        freqInfo.str()
    );
    yOffset += 25;
    std::stringstream tickInfo;
    tickInfo << "Tick: " << m_gameData.tick;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        tickInfo.str()
    );
}

void TimeInfoWindow::updateSpecificData(const GameData& gameData) {
    (void)gameData;
}

} // namespace GUI
