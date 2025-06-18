/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** User Interface implementation using the factory pattern
*/

#include <algorithm>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "UserInterface.hpp"

namespace GUI {

UserInterface::UserInterface(std::shared_ptr<IGuiLib> guiLib)
    : m_guiLib(guiLib),
      m_screenWidth(1920),
      m_screenHeight(1080),
      m_isDragging(false),
      m_mouseCapture(false) {
    m_windowFactory = std::make_shared<UIWindowFactory>(guiLib);
}

void UserInterface::initialize(int screenWidth, int screenHeight) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_windowFactory->createAllWindows(screenWidth, screenHeight);
    m_isDragging = false;
}

void UserInterface::render() {
    m_windowFactory->renderAllWindows();
}

void UserInterface::updateData(const GameData& gameData) {
    m_windowFactory->updateAllWindows(gameData);
}

void UserInterface::addLogMessage(const std::string& message) {
    m_windowFactory->addLogMessage(message);
}

void UserInterface::clearLogs() {
    // Logs functionality has been removed
}

void UserInterface::setSelectedTile(int x, int y) {
    m_windowFactory->setSelectedTile(x, y);
}

void UserInterface::setViewMode(int mode) {
    m_windowFactory->setViewMode(mode);
}

bool UserInterface::toggleWindowVisibility(const std::string& windowId, bool visible) {
    auto window = m_windowFactory->getWindow(windowId);
    if (window) {
        window->setVisible(visible);
        if (visible) {
            addLogMessage("Fenêtre " + windowId + " ouverte");
        } else {
            addLogMessage("Fenêtre " + windowId + " fermée");
        }
        return true;
    }
    return false;
}

bool UserInterface::handleMouseEvents() {
    ZappyTypes::Vector2 mousePosition = m_guiLib->GetMousePosition();
    m_mouseCapture = false;
    bool mouseOverUI = isMouseOverUI();
    if (mouseOverUI) {
        m_mouseCapture = true;
        if (m_guiLib->IsMouseButtonPressed(0) && !m_isDragging) {
            bool startedDragging = m_windowFactory->handleWindowDragging(mousePosition);
            if (startedDragging) {
                m_isDragging = true;
            }
        } else if (m_guiLib->IsMouseButtonDown(0) && m_isDragging) {
            m_windowFactory->updateWindowDragging(mousePosition);
        }
        if (m_guiLib->IsMouseButtonPressed(0) ||
            m_guiLib->IsMouseButtonReleased(0) ||
            m_guiLib->IsMouseButtonDown(0)) {
            m_mouseCapture = true;
        }
    }
    if (!m_guiLib->IsMouseButtonDown(0) && m_isDragging) {
        m_windowFactory->stopWindowDragging();
        m_isDragging = false;
    }
    return m_mouseCapture;
}

bool UserInterface::isMouseOverUI() const {
    ZappyTypes::Vector2 mousePosition = m_guiLib->GetMousePosition();
    return m_windowFactory->isMouseOverWindow(mousePosition);
}

bool UserInterface::hasHandledMouseEvent() const {
    return m_mouseCapture;
}

} // namespace GUI
