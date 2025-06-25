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
    : _guiLib(guiLib), _title(title), _position({0, 0}), _dimensions({0, 0}), _visible(false),
      _showWindowBox(true), _dragging(false), _dragOffset({0, 0}) { }

void AUIWindow::initialize(const ZappyTypes::Vector2& position,
    const ZappyTypes::Vector2& dimensions) {
    _position = position;
    _dimensions = dimensions;
}

bool AUIWindow::render() {
    if (!_visible)
        return true;
    if (_showWindowBox) {
        ZappyTypes::Rectangle windowRect = {_position.x, _position.y,
            _dimensions.x, _dimensions.y};
        bool closeButtonClicked = _guiLib->DrawWindowBox(
            windowRect.x, windowRect.y, windowRect.width, windowRect.height, _title );
        if (closeButtonClicked) {
            _visible = false;
            return false;
        }
    }
    renderContent();
    return true;
}

void AUIWindow::updateData(std::shared_ptr<const GameState> gameState) {
    _gameState = gameState;

}

void AUIWindow::setPosition(const ZappyTypes::Vector2& position) {
    _position = position;
}

ZappyTypes::Vector2 AUIWindow::getPosition() const {
    return _position;
}

ZappyTypes::Vector2 AUIWindow::getDimensions() const {
    return _dimensions;
}

bool AUIWindow::isVisible() const {
    return _visible;
}

void AUIWindow::setVisible(bool visible) {
    _visible = visible;
}

void AUIWindow::toggleVisibility() {
    _visible = !_visible;
}

bool AUIWindow::startDragging(const ZappyTypes::Vector2& mousePosition) {
    ZappyTypes::Rectangle titleBarRect = { _position.x, _position.y, _dimensions.x, 20};
    if (_guiLib->CheckCollisionPointRec(mousePosition, titleBarRect)) {
        _dragging = true;
        _dragOffset.x = mousePosition.x - _position.x;
        _dragOffset.y = mousePosition.y - _position.y;
        return true;
    }
    return false;
}

void AUIWindow::updateDragging(const ZappyTypes::Vector2& mousePosition) {
    if (_dragging) {
        _position.x = mousePosition.x - _dragOffset.x;
        _position.y = mousePosition.y - _dragOffset.y;
    }
}

void AUIWindow::stopDragging() {
    _dragging = false;
}

bool AUIWindow::isDragging() const {
    return _dragging;
}

bool AUIWindow::isPositionInWindow(const ZappyTypes::Vector2& position) const {
    ZappyTypes::Rectangle windowRect = {_position.x, _position.y, _dimensions.x, _dimensions.y};
    return _guiLib->CheckCollisionPointRec(position, windowRect);
}

void AUIWindow::setShowWindowBox(bool show) {
    _showWindowBox = show;
}

} // namespace GUI
