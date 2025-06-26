/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicsManager
*/

#include <memory>
#include "GraphicsManager.hpp"

GraphicsManager::GraphicsManager(std::shared_ptr<IGraphicsLib> graphics)
    : _graphics(graphics) {}

void GraphicsManager::setGraphics(std::shared_ptr<IGraphicsLib> graphics) {
    _graphics = graphics;
}

std::shared_ptr<IGraphicsLib> GraphicsManager::getGraphics() const {
    return _graphics;
}

bool GraphicsManager::isGraphicsAvailable() const {
    return _graphics != nullptr;
}
