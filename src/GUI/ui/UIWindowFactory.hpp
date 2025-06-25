/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UIWindowFactory - Factory pour créer les différentes fenêtres UI
*/

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "windows/IUIWindow.hpp"
#include "../gameController/GameState.hpp"
#include "../../Shared/IGuiLib.hpp"

class NetworkManager;

namespace GUI {

class UIWindowFactory {
public:
    explicit UIWindowFactory(std::shared_ptr<IGuiLib> guiLib);
    ~UIWindowFactory() = default;
    void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);
    void createAllWindows(int screenWidth, int screenHeight);
    std::shared_ptr<IUIWindow> getWindow(const std::string& windowId);
    void renderAllWindows();
    void updateAllWindows(std::shared_ptr<const GameState> gameState,
        int mapWidth, int mapHeight, float gameTime, int frequency, int gameTick);
    void setSelectedTile(int x, int y);
    void setSelectedPlayer(int playerId);


    std::shared_ptr<NetworkManager> getNetworkManager() const;

    void setViewMode(int mode);

    bool handleWindowDragging(const ZappyTypes::Vector2& mousePosition);
    void updateWindowDragging(const ZappyTypes::Vector2& mousePosition);
    void stopWindowDragging();
    bool isMouseOverWindow(const ZappyTypes::Vector2& mousePosition) const;

private:
    void createWindow(const std::string& id, const ZappyTypes::Vector2& position,
        const ZappyTypes::Vector2& dimensions, bool visible);
    void renderRegularWindows();
    void renderMenuWindow();
    std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>> getWindowsInZOrder();
    bool tryStartDraggingWindowInZOrder(const std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>>& windows,
        const ZappyTypes::Vector2& mousePosition);

    std::shared_ptr<IGuiLib> _guiLib;
        std::shared_ptr<NetworkManager> _networkManager;
    std::unordered_map<std::string, std::shared_ptr<IUIWindow>> _windows;
    struct {
        int x;
        int y;
        bool selected;
    } _selectedTile;

    int _currentViewMode;
    std::vector<std::string> _viewModes;
};

} // namespace GUI
