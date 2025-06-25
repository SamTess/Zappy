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
        void setFPS(int fps) { _fps = fps; }
        int getFPS() const { return _fps; }
    private:
        void renderContent() override;
        int _fps = 60;
        int _frequency = 100;

};

} // namespace GUI
