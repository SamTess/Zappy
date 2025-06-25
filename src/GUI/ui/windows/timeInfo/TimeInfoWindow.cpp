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

float TimeInfoWindow::renderFpsInfo(float yOffset) {
    std::stringstream fpsInfo;
    fpsInfo << "FPS: " << m_fps;
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        fpsInfo.str()
    );
    return yOffset + 25;
}

float TimeInfoWindow::renderTimeInfo(float yOffset) {
    if (!m_dataProvider)
        return yOffset;
    std::stringstream timeInfo;
    timeInfo << "Temps: " << std::fixed << std::setprecision(2) << m_dataProvider->getGameTime();
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        timeInfo.str()
    );
    return yOffset + 25;
}

float TimeInfoWindow::renderFrequencyInfo(float yOffset) {
    if (!m_dataProvider)
        return yOffset;
    std::stringstream freqInfo;
    freqInfo << "Fréquence: " << m_dataProvider->getFrequency() << " Hz";
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        freqInfo.str()
    );
    return yOffset + 25;
}

float TimeInfoWindow::renderTickInfo(float yOffset) {
    if (!m_dataProvider)
        return yOffset;
    std::stringstream tickInfo;
    tickInfo << "Tick: " << m_dataProvider->getGameTick();
    m_guiLib->DrawLabel(
        m_position.x + 10,
        yOffset,
        m_dimensions.x - 20,
        20,
        tickInfo.str()
    );
    return yOffset + 25;
}

void TimeInfoWindow::renderContent() {
    float yOffset = m_position.y + 30;
    yOffset = renderFpsInfo(yOffset);
    if (m_dataProvider) {
        yOffset = renderTimeInfo(yOffset);
        yOffset = renderFrequencyInfo(yOffset);
        yOffset = renderTickInfo(yOffset);
    }
}

} // namespace GUI
