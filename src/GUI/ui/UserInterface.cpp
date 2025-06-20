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
#include "../gameController/GameState.hpp"

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

void UserInterface::updateDataFromGameState(
    std::shared_ptr<const GameState> gameState,
    int mapWidth,
    int mapHeight,
    float gameTime,
    int frequency,
    int gameTick
) {
    // Utiliser directement le GameState pour mettre à jour l'interface
    m_windowFactory->updateAllWindows(gameState, mapWidth, mapHeight, gameTime, frequency, gameTick);
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
        handleUIMouseInteraction(mousePosition);
    }
    handleDragEndIfNeeded();
    return m_mouseCapture;
}

void UserInterface::handleUIMouseInteraction(const ZappyTypes::Vector2& mousePosition) {
    if (m_guiLib->IsMouseButtonPressed(0) && !m_isDragging)
        startDraggingIfPossible(mousePosition);
    else if (m_guiLib->IsMouseButtonDown(0) && m_isDragging)
        m_windowFactory->updateWindowDragging(mousePosition);
    if (m_guiLib->IsMouseButtonPressed(0) ||
        m_guiLib->IsMouseButtonReleased(0) ||
        m_guiLib->IsMouseButtonDown(0)) {
        m_mouseCapture = true;
    }
}

void UserInterface::startDraggingIfPossible(const ZappyTypes::Vector2& mousePosition) {
    bool startedDragging = m_windowFactory->handleWindowDragging(mousePosition);
    if (startedDragging) {
        m_isDragging = true;
    }
}

void UserInterface::handleDragEndIfNeeded() {
    if (!m_guiLib->IsMouseButtonDown(0) && m_isDragging) {
        m_windowFactory->stopWindowDragging();
        m_isDragging = false;
    }
}

bool UserInterface::isMouseOverUI() const {
    ZappyTypes::Vector2 mousePosition = m_guiLib->GetMousePosition();
    return m_windowFactory->isMouseOverWindow(mousePosition);
}

bool UserInterface::hasHandledMouseEvent() const {
    return m_mouseCapture;
}

} // namespace GUI
