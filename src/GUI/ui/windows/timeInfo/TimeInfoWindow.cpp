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
    _guiLib->DrawLabel(_position.x + 10, yOffset, _dimensions.x - 20, 20, fpsInfo.str());
    yOffset += 25;
    std::stringstream freqInfo;
    _frequency = _gameState->getTimeUnit();
    freqInfo << "Fréquence: " << _frequency << "/s";
    _guiLib->DrawLabel(_position.x + 10, yOffset, _dimensions.x - 20, 20, freqInfo.str());
}
} // namespace GUI
