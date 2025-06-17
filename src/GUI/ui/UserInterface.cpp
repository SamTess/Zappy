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
#include "windows/logs/LogsWindow.hpp"

namespace GUI {

UserInterface::UserInterface(std::shared_ptr<IGuiLib> guiLib)
    : m_guiLib(guiLib),
      m_screenWidth(800),
      m_screenHeight(600),
      m_isDragging(false) {
    m_windowFactory = std::make_unique<UIWindowFactory>(guiLib);
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
    auto logsWindow = std::dynamic_pointer_cast<LogsWindow>(
        m_windowFactory->getWindow("logs"));
    if (logsWindow) {
        logsWindow->clearLogs();
        addLogMessage("Logs effacés");
    }
}

void UserInterface::setSelectedTile(int x, int y) {
    m_windowFactory->setSelectedTile(x, y);
}

void UserInterface::addBroadcast(const std::string& team, const std::string& message) {
    m_windowFactory->addBroadcast(team, message);
}

void UserInterface::setViewMode(int mode) {
    m_windowFactory->setViewMode(mode);
}

void UserInterface::handleMouseEvents() {
    ZappyTypes::Vector2 mousePosition = m_guiLib->GetMousePosition();
    if (m_guiLib->IsMouseButtonPressed(0) && !m_isDragging) {
        m_windowFactory->handleWindowDragging(mousePosition);
        m_isDragging = true;
    }
    if (m_guiLib->IsMouseButtonDown(0) && m_isDragging)
        m_windowFactory->updateWindowDragging(mousePosition);
    if (!m_guiLib->IsMouseButtonDown(0) && m_isDragging) {
        m_windowFactory->stopWindowDragging();
        m_isDragging = false;
    }
}

} // namespace GUI
