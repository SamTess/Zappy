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
#include "../../Shared/IGuiLib.hpp"

namespace GUI {

class UserInterface {
    public:

        explicit UserInterface(std::shared_ptr<IGuiLib> guiLib);
        ~UserInterface() = default;
        void initialize(int screenWidth, int screenHeight);
        void render();
        void updateDataFromGameState(std::shared_ptr<const GameState> gameState);
        void setSelectedTile(int x, int y);
        void setSelectedPlayer(int playerId);
        void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);
        bool toggleWindowVisibility(const std::string& windowId, bool visible);
        bool handleMouseEvents();
        bool isMouseOverUI() const;
        bool hasHandledMouseEvent() const;
        std::shared_ptr<IUIWindow> getWindow(const std::string& windowId);

    private:
        void handleUIMouseInteraction(const ZappyTypes::Vector2& mousePosition);
        void startDraggingIfPossible(const ZappyTypes::Vector2& mousePosition);
        void handleDragEndIfNeeded();
        std::shared_ptr<IGuiLib> _guiLib;
        std::shared_ptr<UIWindowFactory> _windowFactory;
        int _screenWidth;
        int _screenHeight;
        bool _isDragging;
        bool _mouseCapture;
};

} // namespace GUI
