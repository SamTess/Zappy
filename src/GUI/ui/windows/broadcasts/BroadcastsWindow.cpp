/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** BroadcastsWindow implementation
*/

#include <sstream>
#include <string>
#include <memory>
#include <algorithm>
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
    int maxVisibleItems = calculateVisibleItemCount(panelHeight, lineHeight);
    int startIndex = calculateStartIndex(maxVisibleItems);

    renderBroadcastMessages(view, startIndex, lineHeight, contentWidth);
}

int GUI::BroadcastsWindow::calculateVisibleItemCount(float panelHeight, float lineHeight) {
    return static_cast<int>(panelHeight / lineHeight);
}

int GUI::BroadcastsWindow::calculateStartIndex(int maxVisibleItems) {
    if (m_broadcasts.size() > static_cast<size_t>(maxVisibleItems)) {
        return m_broadcasts.size() - maxVisibleItems;
    }
    return 0;
}

void GUI::BroadcastsWindow::renderBroadcastMessages(const ZappyTypes::Rectangle& view,
    int startIndex, float lineHeight,
    float contentWidth) {
    for (size_t i = startIndex; i < m_broadcasts.size(); i++) {
        float textY = view.y + ((i - startIndex) * lineHeight);
        std::string message = formatBroadcastMessage(m_broadcasts[i]);
        m_guiLib->DrawLabel(view.x + 5, textY, contentWidth - 10, lineHeight, message);
    }
}

std::string GUI::BroadcastsWindow::formatBroadcastMessage(const Broadcast& broadcast) {
    return broadcast.team + ": " + broadcast.message + " (" +
           std::to_string(static_cast<int>(broadcast.timeLeft)) + "s)";
}

void GUI::BroadcastsWindow::updateBroadcastMessages() {
    const float frameDuration = 0.016f;
    for (auto it = m_broadcasts.begin(); it != m_broadcasts.end();) {
        it->timeLeft -= frameDuration;
        if (it->timeLeft <= 0.0f) {
            it = m_broadcasts.erase(it);
        } else {
            ++it;
        }
    }
}

void GUI::BroadcastsWindow::addBroadcast(const std::string& team, const std::string& message) {
    Broadcast newBroadcast;
    newBroadcast.team = team;
    newBroadcast.message = message;
    newBroadcast.timeLeft = 15.0f;
    m_broadcasts.push_back(newBroadcast);
    while (m_broadcasts.size() > m_maxBroadcasts)
        m_broadcasts.pop_front();
    setVisible(true);
}

void GUI::BroadcastsWindow::clearBroadcasts() {
    m_broadcasts.clear();
}

} // namespace GUI
