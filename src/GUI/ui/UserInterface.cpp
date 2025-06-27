/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UserInterface
*/

#include <algorithm>
#include <string>
#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "UserInterface.hpp"
#include "context/UIContext.hpp"
#include "windows/timeInfo/TimeInfoWindow.hpp"
#include "../gameController/GameState.hpp"

namespace GUI {

UserInterface::UserInterface(std::shared_ptr<IGuiLib> guiLib)
   : _guiLib(guiLib), _screenWidth(1920), _screenHeight(1080),
     _isDragging(false), _mouseCapture(false) {
    _windowFactory = std::make_shared<UIWindowFactory>(guiLib);
}

void UserInterface::initialize(int screenWidth, int screenHeight) {
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _windowFactory->createAllWindows(screenWidth, screenHeight, _windowFactory);
    auto uiContext = std::make_shared<UIContext>(_windowFactory, _commandSender);
    _windowFactory->setUIContext(uiContext);
    _isDragging = false;
}

void UserInterface::render() {
    _windowFactory->renderAllWindows(_windowFactory);
}

void UserInterface::updateDataFromGameState(std::shared_ptr<const GameState> gameState) {
    _windowFactory->updateAllWindows(gameState);
}

void UserInterface::updateTimeData(float gameTime, int frequency, int gameTick) {
    auto timeInfoWindow = std::dynamic_pointer_cast<GUI::TimeInfoWindow>(getWindow("timeInfo"));
    (void)timeInfoWindow;
    _windowFactory->updateTimeData(gameTime, frequency, gameTick);
}

void UserInterface::setSelectedTile(int x, int y) {
    _windowFactory->setSelectedTile(x, y);
}

void UserInterface::setSelectedPlayer(int playerId) {
    _windowFactory->setSelectedPlayer(playerId);
}

bool UserInterface::toggleWindowVisibility(const std::string& windowId, bool visible) {
    auto window = _windowFactory->getWindow(windowId);
    if (window) {
        window->setVisible(visible);
        return true;
    }
    return false;
}

bool UserInterface::handleMouseEvents() {
    ZappyTypes::Vector2 mousePosition = _guiLib->GetMousePosition();
    _mouseCapture = false;
    bool mouseOverUI = isMouseOverUI();
    if (mouseOverUI) {
        _mouseCapture = true;
        handleUIMouseInteraction(mousePosition);
    }
    handleDragEndIfNeeded();
    return _mouseCapture;
}

void UserInterface::handleUIMouseInteraction(const ZappyTypes::Vector2& mousePosition) {
    if (_guiLib->IsMouseButtonPressed(0) && !_isDragging)
        startDraggingIfPossible(mousePosition);
    else if (_guiLib->IsMouseButtonDown(0) && _isDragging)
        _windowFactory->updateWindowDragging(mousePosition);
    if (_guiLib->IsMouseButtonPressed(0) ||
        _guiLib->IsMouseButtonReleased(0) ||
        _guiLib->IsMouseButtonDown(0)) {
        _mouseCapture = true;
    }
}

void UserInterface::startDraggingIfPossible(const ZappyTypes::Vector2& mousePosition) {
    bool startedDragging = _windowFactory->handleWindowDragging(mousePosition);
    if (startedDragging)
        _isDragging = true;
}

void UserInterface::handleDragEndIfNeeded() {
    if (!_guiLib->IsMouseButtonDown(0) && _isDragging) {
        _windowFactory->stopWindowDragging();
        _isDragging = false;
    }
}

bool UserInterface::isMouseOverUI() const {
    ZappyTypes::Vector2 mousePosition = _guiLib->GetMousePosition();
    return _windowFactory->isMouseOverWindow(mousePosition);
}

bool UserInterface::hasHandledMouseEvent() const {
    return _mouseCapture;
}

std::shared_ptr<GUI::IUIWindow> UserInterface::getWindow(const std::string& windowId) {
    return _windowFactory->getWindow(windowId);
}

void UserInterface::setCommandSender(std::shared_ptr<INetworkCommandSender> sender) {
    _commandSender = sender;
    _windowFactory->setCommandSender(sender);
}

void UserInterface::setCommandExecutor(std::shared_ptr<ICommandExecutor> executor) {
    _windowFactory->setCommandExecutor(executor);
}

} // namespace GUI
