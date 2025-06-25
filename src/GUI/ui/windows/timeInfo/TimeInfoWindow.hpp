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

    void setFPS(int fps) { m_fps = fps; }
    int getFPS() const { return m_fps; }

protected:
    void renderContent() override;

private:
    int m_fps = 60;
    void renderFpsInfo(float& yOffset);
    void renderTimeInfo(float& yOffset);
    void renderFrequencyInfo(float& yOffset);
    void renderTickInfo(float& yOffset);
};

} // namespace GUI
