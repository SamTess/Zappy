/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** User Interface implementation
*/

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include "UIWindowFactory.hpp"
#include "../gameController/GameState.hpp"
#include "../shared/services/ComponentCoordinator.hpp"
#include "../../Shared/IGuiLib.hpp"

namespace GUI {

class UserInterface : public IUINotifier {
public:
    explicit UserInterface(std::shared_ptr<IGuiLib> guiLib);
    ~UserInterface() = default;
    void initialize(int screenWidth, int screenHeight);
    void render();
    void updateDataFromGameState(
        std::shared_ptr<const GameState> gameState,
        int mapWidth,
        int mapHeight,
        float gameTime,
        int frequency,
        int gameTick
    );
    void setSelectedTile(int x, int y);
    void setSelectedPlayer(int playerId);
    void addBroadcast(const std::string& team, const std::string& message);
    void notifyConnectionStatus(bool connected) override;
    void setCommandSender(std::shared_ptr<INetworkCommandSender> sender) override;
    void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor);
    void setViewMode(int mode);
    bool toggleWindowVisibility(const std::string& windowId, bool visible);
    bool handleMouseEvents();
    bool isMouseOverUI() const;
    bool hasHandledMouseEvent() const;
    std::shared_ptr<IUIWindow> getWindow(const std::string& windowId);

private:
    void handleUIMouseInteraction(const ZappyTypes::Vector2& mousePosition);
    void startDraggingIfPossible(const ZappyTypes::Vector2& mousePosition);
    void handleDragEndIfNeeded();
    std::shared_ptr<IGuiLib> m_guiLib;
    std::shared_ptr<UIWindowFactory> m_windowFactory;
    std::shared_ptr<INetworkCommandSender> m_commandSender;
    int m_screenWidth;
    int m_screenHeight;
    bool m_isDragging;
    bool m_mouseCapture;
};

} // namespace GUI
