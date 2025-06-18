/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Abstract class for UI Windows implementation
*/

#include "AUIWindow.hpp"
#include <iostream>
#include <string>

namespace GUI {

AUIWindow::AUIWindow(std::shared_ptr<IGuiLib> guiLib, const std::string& title)
    : m_guiLib(guiLib),
      m_title(title),
      m_position({0, 0}),
      m_dimensions({0, 0}),
      m_scrollPosition({0, 0}),
      m_visible(false),
      m_dragging(false),
      m_dragOffset({0, 0}) { }

void AUIWindow::initialize(const ZappyTypes::Vector2& position,
                         const ZappyTypes::Vector2& dimensions) {
    m_position = position;
    m_dimensions = dimensions;
}

bool AUIWindow::render() {
    if (!m_visible)
        return true;
    ZappyTypes::Rectangle windowRect = {
        m_position.x, m_position.y,
        m_dimensions.x, m_dimensions.y
    };
    bool closeButtonClicked = m_guiLib->DrawWindowBox(
        windowRect.x, windowRect.y,
        windowRect.width, windowRect.height,
        m_title
    );
    if (closeButtonClicked) {
        m_visible = false;
        return false;
    } else {
        renderContent();
        return true;
    }
}

void AUIWindow::updateData(const GameData& gameData) {
    m_gameData = gameData;
    updateSpecificData(gameData);
}

void AUIWindow::setPosition(const ZappyTypes::Vector2& position) {
    m_position = position;
}

ZappyTypes::Vector2 AUIWindow::getPosition() const {
    return m_position;
}

ZappyTypes::Vector2 AUIWindow::getDimensions() const {
    return m_dimensions;
}

bool AUIWindow::isVisible() const {
    return m_visible;
}

void AUIWindow::setVisible(bool visible) {
    m_visible = visible;
}

void AUIWindow::toggleVisibility() {
    m_visible = !m_visible;
}

bool AUIWindow::startDragging(const ZappyTypes::Vector2& mousePosition) {
    ZappyTypes::Rectangle titleBarRect = {
        m_position.x, m_position.y,
        m_dimensions.x, 20.0f
    };
    if (m_guiLib->CheckCollisionPointRec(mousePosition, titleBarRect)) {
        m_dragging = true;
        m_dragOffset.x = mousePosition.x - m_position.x;
        m_dragOffset.y = mousePosition.y - m_position.y;
        return true;
    }
    return false;
}

void AUIWindow::updateDragging(const ZappyTypes::Vector2& mousePosition) {
    if (m_dragging) {
        m_position.x = mousePosition.x - m_dragOffset.x;
        m_position.y = mousePosition.y - m_dragOffset.y;
    }
}

void AUIWindow::stopDragging() {
    m_dragging = false;
}

bool AUIWindow::isDragging() const {
    return m_dragging;
}

bool AUIWindow::isPositionInWindow(const ZappyTypes::Vector2& position) const {
    ZappyTypes::Rectangle windowRect = {
        m_position.x, m_position.y,
        m_dimensions.x, m_dimensions.y
    };
    return m_guiLib->CheckCollisionPointRec(position, windowRect);
}

} // namespace GUI
