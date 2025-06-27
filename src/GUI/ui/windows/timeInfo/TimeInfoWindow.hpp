/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** TimeInfoWindow
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

    protected:
        void renderContent() override;

    private:
        int _fps = 60;
        float renderFpsInfo(float yOffset);
        float renderTimeInfo(float yOffset);
        float renderFrequencyInfo(float yOffset);
};

} // namespace GUI
