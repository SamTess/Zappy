/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TimeInfoWindow definition
*/

#pragma once

#include <memory>
#include "../AUIWindow.hpp"

namespace GUI {

class TimeInfoWindow : public AUIWindow {
public:
    explicit TimeInfoWindow(std::shared_ptr<IGuiLib> guiLib);
    ~TimeInfoWindow() = default;

protected:
    void renderContent() override;
    void updateSpecificData(std::shared_ptr<const GameState> gameState,
                          int mapWidth, int mapHeight,
                          float gameTime, int frequency, int gameTick) override;
};

} // namespace GUI
