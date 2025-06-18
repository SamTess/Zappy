/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** BroadcastsWindow implementation
*/

#include <sstream>
#include <string>
#include <memory>
#include <algorithm> // Pour std::min, std::max
#include "BroadcastsWindow.hpp"

namespace GUI {

BroadcastsWindow::BroadcastsWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Broadcasts"),
      m_maxBroadcasts(20) {
}

void BroadcastsWindow::renderContent() {
    const float lineHeight = 20.0f;
    const float panelWidth = m_dimensions.x - 20;
    const float panelHeight = m_dimensions.y - 40;
    const float contentWidth = panelWidth - 20;
    ZappyTypes::Rectangle view = {
        m_position.x + 10,
        m_position.y + 30,
        panelWidth,
        panelHeight
    };
    int maxVisibleItems = static_cast<int>(panelHeight / lineHeight);
    int startIndex = 0;
    if (m_broadcasts.size() > static_cast<size_t>(maxVisibleItems)) {
        startIndex = m_broadcasts.size() - maxVisibleItems;
    }
    for (size_t i = startIndex; i < m_broadcasts.size(); i++) {
        float textY = view.y + ((i - startIndex) * lineHeight);
        std::string message = m_broadcasts[i].team + ": " + m_broadcasts[i].message + " (" +
                             std::to_string(static_cast<int>(m_broadcasts[i].timeLeft)) + "s)";
        m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    }
}

void BroadcastsWindow::updateSpecificData(const GameData& gameData) {
    (void)gameData;
    for (auto it = m_broadcasts.begin(); it != m_broadcasts.end();) {
        it->timeLeft -= 0.016f;
        if (it->timeLeft <= 0.0f) {
            it = m_broadcasts.erase(it);
        } else {
            ++it;
        }
    }
}

void BroadcastsWindow::addBroadcast(const std::string& team, const std::string& message) {
    Broadcast newBroadcast;
    newBroadcast.team = team;
    newBroadcast.message = message;
    newBroadcast.timeLeft = 15.0f;
    m_broadcasts.push_back(newBroadcast);
    while (m_broadcasts.size() > m_maxBroadcasts)
        m_broadcasts.pop_front();
    setVisible(true);
}

void BroadcastsWindow::clearBroadcasts() {
    m_broadcasts.clear();
}

} // namespace GUI
