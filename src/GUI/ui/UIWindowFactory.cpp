/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UIWindowFactory implementation
*/

#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <utility>
#include "UIWindowFactory.hpp"
#include "windows/tileInfo/TileInfoWindow.hpp"
#include "windows/playerInfo/PlayerInfoWindow.hpp"
#include "windows/broadcasts/BroadcastsWindow.hpp"
#include "windows/controls/ControlsWindow.hpp"
#include "windows/timeInfo/TimeInfoWindow.hpp"
#include "windows/menu/MenuWindow.hpp"

namespace GUI {

UIWindowFactory::UIWindowFactory(std::shared_ptr<IGuiLib> guiLib)
    : m_guiLib(guiLib), m_currentViewMode(0) {
    m_selectedTile = {0, 0, false};
    m_viewModes = {"Vue standard", "Vue aérienne", "Vue wireframe", "Vue ressources"};
}

void UIWindowFactory::createAllWindows(int, int) {
    ZappyTypes::Vector2 tileInfoPos = {10, 810};
    ZappyTypes::Vector2 playerInfoPos = {10, 500};
    ZappyTypes::Vector2 broadcastsPos = {1520, 540};
    ZappyTypes::Vector2 controlsPos = {1520, 860};
    ZappyTypes::Vector2 timeInfoPos = {1450, 10};
    ZappyTypes::Vector2 menuPos = {10, 10};
    ZappyTypes::Vector2 standardDimensions = {400, 300};
    ZappyTypes::Vector2 timeInfoDimensions = {250, 150};
    ZappyTypes::Vector2 controleDimensions = {400, 200};
    ZappyTypes::Vector2 menuDimensionsLarger = {40, 40};

    auto tileInfoWindow = std::make_shared<TileInfoWindow>(m_guiLib);
    tileInfoWindow->initialize(tileInfoPos, standardDimensions);
    tileInfoWindow->setVisible(false);
    m_windows["tileInfo"] = tileInfoWindow;

    auto playerInfoWindow = std::make_shared<PlayerInfoWindow>(m_guiLib);
    playerInfoWindow->initialize(playerInfoPos, standardDimensions);
    playerInfoWindow->setVisible(false);
    m_windows["playerInfo"] = playerInfoWindow;

    auto broadcastsWindow = std::make_shared<BroadcastsWindow>(m_guiLib);
    broadcastsWindow->initialize(broadcastsPos, standardDimensions);
    broadcastsWindow->setVisible(false);
    m_windows["broadcasts"] = broadcastsWindow;

    auto controlsWindow = std::make_shared<ControlsWindow>(m_guiLib);
    controlsWindow->initialize(controlsPos, controleDimensions);
    controlsWindow->setVisible(false);
    m_windows["controls"] = controlsWindow;

    auto timeInfoWindow = std::make_shared<TimeInfoWindow>(m_guiLib);
    timeInfoWindow->initialize(timeInfoPos, timeInfoDimensions);
    timeInfoWindow->setVisible(false);
    m_windows["timeInfo"] = timeInfoWindow;

    auto menuWindow = std::make_shared<MenuWindow>(m_guiLib);
    menuWindow->initialize(menuPos, menuDimensionsLarger);
    menuWindow->setVisible(true);
    m_windows["menu"] = menuWindow;
    menuWindow->setUIWindowFactory(shared_from_this());
}

std::shared_ptr<IUIWindow> UIWindowFactory::getWindow(const std::string& windowId) {
    auto it = m_windows.find(windowId);
    if (it != m_windows.end()) {
        return it->second;
    }
    return nullptr;
}

void UIWindowFactory::renderAllWindows() {
    std::string menuId = "menu";
    for (auto& pair : m_windows) {
        if (pair.first != menuId && pair.second->isVisible()) {
            bool keepWindowOpen = pair.second->render();
            if (!keepWindowOpen) {
                pair.second->setVisible(false);
            }
        }
    }
    auto menuIt = m_windows.find(menuId);
    if (menuIt != m_windows.end()) {
        if (menuIt->second->isVisible()) {
            bool keepWindowOpen = menuIt->second->render();
            if (!keepWindowOpen) {
                menuIt->second->setVisible(false);
            }
        } else {
            menuIt->second->setVisible(true);
            menuIt->second->render();
        }
    } else {
        ZappyTypes::Vector2 menuPos = {10, 10};
        ZappyTypes::Vector2 menuDimensions = {40, 40};
        auto menuWindow = std::make_shared<MenuWindow>(m_guiLib);
        menuWindow->initialize(menuPos, menuDimensions);
        menuWindow->setVisible(true);
        m_windows[menuId] = menuWindow;
        menuWindow->setUIWindowFactory(shared_from_this());
    }
}

void UIWindowFactory::updateAllWindows(const GameData& gameData) {
    for (auto& pair : m_windows) {
        pair.second->updateData(gameData);
    }
}

void UIWindowFactory::setSelectedTile(int x, int y) {
    m_selectedTile = {x, y, true};
    auto tileInfoWindow = std::dynamic_pointer_cast<TileInfoWindow>(m_windows["tileInfo"]);
    if (tileInfoWindow) {
        tileInfoWindow->setSelectedTile(x, y);
    }
}

void UIWindowFactory::addBroadcast(const std::string& team, const std::string& message) {
    auto broadcastsWindow = std::dynamic_pointer_cast<BroadcastsWindow>(m_windows["broadcasts"]);
    if (broadcastsWindow)
        broadcastsWindow->addBroadcast(team, message);
}

void UIWindowFactory::setViewMode(int mode) {
    if (mode >= 0 && mode < static_cast<int>(m_viewModes.size())) {
        m_currentViewMode = mode;
        auto controlsWindow = std::dynamic_pointer_cast<ControlsWindow>(m_windows["controls"]);
        if (controlsWindow)
            controlsWindow->setViewMode(mode);
    }
}

bool UIWindowFactory::handleWindowDragging(const ZappyTypes::Vector2& mousePosition) {
    std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>> windowsVec;
    windowsVec.reserve(m_windows.size());
    for (const auto& pair : m_windows)
        windowsVec.push_back(pair);
    for (auto it = windowsVec.rbegin(); it != windowsVec.rend(); ++it) {
        if (it->second->isVisible() && it->second->startDragging(mousePosition))
            return true;
    }
    return false;
}

void UIWindowFactory::updateWindowDragging(const ZappyTypes::Vector2& mousePosition) {
    for (auto& pair : m_windows) {
        if (pair.second->isDragging())
            pair.second->updateDragging(mousePosition);
    }
}

void UIWindowFactory::stopWindowDragging() {
    for (auto& pair : m_windows) {
        if (pair.second->isDragging())
            pair.second->stopDragging();
    }
}

bool UIWindowFactory::isMouseOverWindow(const ZappyTypes::Vector2& mousePosition) const {
    for (const auto& pair : m_windows) {
        if (pair.second->isVisible() && pair.second->isPositionInWindow(mousePosition)) {
            return true;
        }
    }
    return false;
}

} // namespace GUI
