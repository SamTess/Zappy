/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UIWindowFactory implementation
*/

#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include "UIWindowFactory.hpp"
#include "windows/tileInfo/TileInfoWindow.hpp"
#include "windows/playerInfo/PlayerInfoWindow.hpp"
#include "windows/mapInfo/MapInfoWindow.hpp"
#include "windows/timeInfo/TimeInfoWindow.hpp"
#include "windows/menu/MenuWindow.hpp"

namespace GUI {

UIWindowFactory::UIWindowFactory(std::shared_ptr<IGuiLib> guiLib)
    : _guiLib(guiLib), _currentViewMode(0) {
    _selectedTile = {0, 0, false};
    _viewModes = {"Vue standard", "Vue aérienne", "Vue wireframe", "Vue ressources"};
}

void UIWindowFactory::setNetworkManager(std::shared_ptr<NetworkManager> networkManager) {
    _networkManager = networkManager;
    auto playerInfoWindow = std::dynamic_pointer_cast<GUI::PlayerInfoWindow>(_windows["playerInfo"]);
    if (playerInfoWindow) {
        playerInfoWindow->setNetworkManager(networkManager);
    }
    auto mapInfoWindow = std::dynamic_pointer_cast<GUI::MapInfoWindow>(_windows["mapInfo"]);
    if (mapInfoWindow) {
        mapInfoWindow->setNetworkManager(networkManager);
    }
}

void UIWindowFactory::createAllWindows(int, int) {
    const struct WindowConfig {
        std::string id;
        ZappyTypes::Vector2 position;
        ZappyTypes::Vector2 dimensions;
        bool initiallyVisible;
    } windowConfigs[] = {
        {"tileInfo", {10, 810}, {400, 300}, false},
        {"playerInfo", {10, 500}, {400, 300}, false},
        {"mapInfo", {1520, 540}, {400, 300}, false},
        {"controls", {1520, 860}, {400, 200}, false},
        {"timeInfo", {1450, 10}, {250, 150}, false},
        {"menu", {10, 10}, {40, 40}, true}
    };
    for (const auto& config : windowConfigs) {
        createWindow(config.id, config.position, config.dimensions, config.initiallyVisible);
    }
    auto menuWindow = std::dynamic_pointer_cast<GUI::MenuWindow>(_windows["menu"]);
    if (menuWindow) {
        menuWindow->setUIWindowFactory(shared_from_this());
    }
}

std::shared_ptr<GUI::IUIWindow> UIWindowFactory::getWindow(const std::string& windowId) {
    auto it = _windows.find(windowId);
    if (it != _windows.end()) {
        return it->second;
    }
    return nullptr;
}

void UIWindowFactory::renderAllWindows() {
    renderRegularWindows();
    renderMenuWindow();
}

void UIWindowFactory::renderRegularWindows() {
    for (auto& pair : _windows) {
        if (pair.first != "menu" && pair.second->isVisible()) {
            bool keepWindowOpen = pair.second->render();
            if (!keepWindowOpen) {
                pair.second->setVisible(false);
            }
        }
    }
}

void UIWindowFactory::renderMenuWindow() {
    auto menuIt = _windows.find("menu");
    if (menuIt != _windows.end()) {
        if (!menuIt->second->isVisible())
            menuIt->second->setVisible(true);
        bool keepWindowOpen = menuIt->second->render();
        if (!keepWindowOpen) {
            menuIt->second->setVisible(false);
        }
    } else {
        ZappyTypes::Vector2 menuPos = {10, 10};
        ZappyTypes::Vector2 menuDimensions = {40, 40};
        auto menuWindow = std::make_shared<GUI::MenuWindow>(_guiLib);
        menuWindow->initialize(menuPos, menuDimensions);
        menuWindow->setVisible(true);
        _windows["menu"] = menuWindow;
        menuWindow->setUIWindowFactory(shared_from_this());
    }
}

void UIWindowFactory::updateAllWindows(std::shared_ptr<const GameState> gameState,
    int mapWidth, int mapHeight, float gameTime, int frequency, int gameTick) {
    for (auto& pair : _windows) {
        pair.second->updateData(gameState);
    }
}

void UIWindowFactory::setSelectedTile(int x, int y) {
    _selectedTile = {x, y, true};
    auto tileInfoWindow = std::dynamic_pointer_cast<GUI::TileInfoWindow>(_windows["tileInfo"]);
    if (tileInfoWindow) {
        tileInfoWindow->setSelectedTile(x, y);
    }
}

void UIWindowFactory::setSelectedPlayer(int playerId) {
    auto playerInfoWindow = std::dynamic_pointer_cast<GUI::PlayerInfoWindow>(_windows["playerInfo"]);
    if (playerInfoWindow) {
        playerInfoWindow->setSelectedPlayer(playerId);
    }
}

std::shared_ptr<NetworkManager> UIWindowFactory::getNetworkManager() const {
    return _networkManager;
}

void UIWindowFactory::setViewMode(int mode) {
    if (mode >= 0 && mode < static_cast<int>(_viewModes.size())) {
        _currentViewMode = mode;
        //auto controlsWindow = std::dynamic_pointer_cast<GUI::ControlsWindow>(_windows["controls"]);
        //if (controlsWindow)
        //    controlsWindow->setViewMode(mode);
    }
}

bool UIWindowFactory::handleWindowDragging(const ZappyTypes::Vector2& mousePosition) {
    std::vector<std::pair<std::string, std::shared_ptr<GUI::IUIWindow>>> windowsInZOrder =
        getWindowsInZOrder();
    return tryStartDraggingWindowInZOrder(windowsInZOrder, mousePosition);
}

std::vector<std::pair<std::string, std::shared_ptr<GUI::IUIWindow>>> UIWindowFactory::getWindowsInZOrder() {
    std::vector<std::pair<std::string, std::shared_ptr<GUI::IUIWindow>>> windowsVec;
    windowsVec.reserve(_windows.size());
    for (const auto& pair : _windows)
        windowsVec.push_back(pair);
    return windowsVec;
}

bool UIWindowFactory::tryStartDraggingWindowInZOrder(
    const std::vector<std::pair<std::string, std::shared_ptr<GUI::IUIWindow>>>& windows,
    const ZappyTypes::Vector2& mousePosition) {
    for (auto it = windows.rbegin(); it != windows.rend(); ++it) {
        if (it->second->isVisible() && it->second->startDragging(mousePosition))
            return true;
    }
    return false;
}

void UIWindowFactory::updateWindowDragging(const ZappyTypes::Vector2& mousePosition) {
    for (auto& [id, window] : _windows) {
        if (window->isDragging()) {
            window->updateDragging(mousePosition);
        }
    }
}

void UIWindowFactory::stopWindowDragging() {
    for (auto& [id, window] : _windows) {
        if (window->isDragging()) {
            window->stopDragging();
        }
    }
}

void UIWindowFactory::createWindow(const std::string& id, const ZappyTypes::Vector2& position,
                                 const ZappyTypes::Vector2& dimensions, bool visible) {
    static const std::unordered_map<std::string, std::function<std::shared_ptr<GUI::IUIWindow>(std::shared_ptr<IGuiLib>)>> windowCreators = {
        {"tileInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::TileInfoWindow>(lib); }},
        {"playerInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::PlayerInfoWindow>(lib); }},
        {"mapInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::MapInfoWindow>(lib); }},
        {"timeInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::TimeInfoWindow>(lib); }},
        {"menu", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::MenuWindow>(lib); }}
    };
    auto it = windowCreators.find(id);
    if (it != windowCreators.end()) {
        auto window = it->second(_guiLib);
        window->initialize(position, dimensions);
        window->setVisible(visible);
        _windows[id] = window;
        if (id == "playerInfo" && _networkManager) {
            auto playerInfoWindow = std::dynamic_pointer_cast<GUI::PlayerInfoWindow>(window);
            if (playerInfoWindow)
                playerInfoWindow->setNetworkManager(_networkManager);
        }
    }
}

bool UIWindowFactory::isMouseOverWindow(const ZappyTypes::Vector2& mousePosition) const {
    for (const auto& pair : _windows) {
        if (pair.second->isVisible() && pair.second->isPositionInWindow(mousePosition)) {
            return true;
        }
    }
    return false;
}

} // namespace GUI
