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
#include "data/UIDataProvider.hpp"
#include "../gameController/GameState.hpp"
#include "../shared/commands/ICommand.hpp"
#include "../shared/commands/ICommandExecutor.hpp"
#include "../../Shared/IGuiLib.hpp"
#include "../shared/commands/INetworkCommandSender.hpp"

class NetworkManager;

namespace GUI {

class IUIContext;

class UIWindowFactory : public std::enable_shared_from_this<UIWindowFactory> {
public:
    explicit UIWindowFactory(std::shared_ptr<IGuiLib> guiLib);
    ~UIWindowFactory() = default;

    void setCommandSender(std::shared_ptr<INetworkCommandSender> sender);
    void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor);
    void createAllWindows(int screenWidth, int screenHeight);
    std::shared_ptr<IUIWindow> getWindow(const std::string& windowId);
    void renderAllWindows();
    void updateAllWindows(std::shared_ptr<const GameState> gameState,
                         int mapWidth, int mapHeight,
                         float gameTime, int frequency, int gameTick);
    void setSelectedTile(int x, int y);
    void setSelectedPlayer(int playerId);
    void addBroadcast(const std::string& team, const std::string& message);
    void setViewMode(int mode);
    bool handleWindowDragging(const ZappyTypes::Vector2& mousePosition);
    void updateWindowDragging(const ZappyTypes::Vector2& mousePosition);
    void stopWindowDragging();
    bool isMouseOverWindow(const ZappyTypes::Vector2& mousePosition) const;
    void setUIContext(std::shared_ptr<IUIContext> uiContext);

private:
    void createWindow(const std::string& id, const ZappyTypes::Vector2& position,
        const ZappyTypes::Vector2& dimensions, bool visible);
    void renderRegularWindows();
    void renderMenuWindow();
    std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>> getWindowsInZOrder();
    bool tryStartDraggingWindowInZOrder(
        const std::vector<std::pair<std::string, std::shared_ptr<IUIWindow>>>& windows,
        const ZappyTypes::Vector2& mousePosition);

    std::shared_ptr<IGuiLib> m_guiLib;
    std::shared_ptr<INetworkCommandSender> m_commandSender;
    std::shared_ptr<ICommandExecutor> m_commandExecutor;
    std::unordered_map<std::string, std::shared_ptr<IUIWindow>> m_windows;
    struct {
        int x;
        int y;
        bool selected;
    } m_selectedTile;

    int m_currentViewMode;
    std::vector<std::string> m_viewModes;
    std::shared_ptr<IUIContext> m_uiContext;
};

} // namespace GUI
