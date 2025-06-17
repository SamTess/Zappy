/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** LogsWindow implementation
*/

#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
#include <chrono>
#include "LogsWindow.hpp"

namespace GUI {

LogsWindow::LogsWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Journal de bord"),
      m_maxLogs(30) {
}

void LogsWindow::renderContent() {
    std::stringstream logsContent;
    for (const auto& log : m_logs) {
        logsContent << log << "\n";
    }
    ZappyTypes::Vector2 contentSize = {m_dimensions.x - 20, static_cast<float>(m_logs.size() * 20)};
    ZappyTypes::Rectangle contentRect = {0, 0, contentSize.x, contentSize.y};
    m_guiLib->DrawScrollPanel(
        m_position.x + 10,
        m_position.y + 30,
        m_dimensions.x - 20,
        m_dimensions.y - 40,
        logsContent.str(),
        contentRect,
        std::make_shared<ZappyTypes::Vector2>(m_scrollPosition)
    );
}

void LogsWindow::updateSpecificData(const GameData& gameData) {
    (void)gameData;
}

void LogsWindow::addLogMessage(const std::string& message) {
    m_logs.push_front(message);
    while (m_logs.size() > m_maxLogs) {
        m_logs.pop_back();
    }
}

void LogsWindow::clearLogs() {
    m_logs.clear();
}

} // namespace GUI
