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
    float yOffset = _position.y + 30;

    std::stringstream fpsInfo;
    fpsInfo << "FPS: " << _fps;
    _guiLib->DrawLabel(
        _position.x + 10,
        yOffset,
        _dimensions.x - 20,
        20,
        fpsInfo.str()
    );
    yOffset += 25;

    std::stringstream timeInfo;
    timeInfo << "Temps: " << std::fixed << std::setprecision(2) << _gameTime;
    _guiLib->DrawLabel(
        _position.x + 10,
        yOffset,
        _dimensions.x - 20,
        20,
        timeInfo.str()
    );
    yOffset += 25;
    std::stringstream freqInfo;
    freqInfo << "Fréquence: " << _frequency << " Hz";
    _guiLib->DrawLabel(
        _position.x + 10,
        yOffset,
        _dimensions.x - 20,
        20,
        freqInfo.str()
    );
    yOffset += 25;
    std::stringstream tickInfo;
    tickInfo << "Tick: " << _gameTick;
    _guiLib->DrawLabel(
        _position.x + 10,
        yOffset,
        _dimensions.x - 20,
        20,
        tickInfo.str()
    );
}

} // namespace GUI
