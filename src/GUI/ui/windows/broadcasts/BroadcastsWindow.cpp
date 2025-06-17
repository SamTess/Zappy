/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** BroadcastsWindow implementation
*/

#include <sstream>
#include <string>
#include <memory>
#include "BroadcastsWindow.hpp"

namespace GUI {

BroadcastsWindow::BroadcastsWindow(std::shared_ptr<IGuiLib> guiLib)
    : AUIWindow(guiLib, "Broadcasts"),
      m_maxBroadcasts(20) {
}

void BroadcastsWindow::renderContent() {
    std::stringstream broadcastsContent;
    for (const auto& broadcast : m_broadcasts) {
        broadcastsContent << broadcast.team << ": " << broadcast.message << " ("
                         << static_cast<int>(broadcast.timeLeft) << "s)\n";
    }
    ZappyTypes::Vector2 contentSize = {
        m_dimensions.x - 20,
        static_cast<float>(m_broadcasts.size() * 20)
    };
    ZappyTypes::Rectangle contentRect = {0, 0, contentSize.x, contentSize.y};
    ZappyTypes::Rectangle view = m_guiLib->DrawScrollPanel(
        m_position.x + 10,
        m_position.y + 30,
        m_dimensions.x - 20,
        m_dimensions.y - 40,
        broadcastsContent.str(),
        contentRect,
        std::make_shared<ZappyTypes::Vector2>(m_scrollPosition)
    );
    (void)view;
}

void BroadcastsWindow::updateSpecificData(const GameData& /*gameData*/) {
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
    m_broadcasts.push_front(newBroadcast);
    while (m_broadcasts.size() > m_maxBroadcasts)
        m_broadcasts.pop_back();
    setVisible(true);
}

void BroadcastsWindow::clearBroadcasts() {
    m_broadcasts.clear();
}

} // namespace GUI
