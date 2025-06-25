/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** PlayerInfoWindow implementation
*/

#pragma once

#include <memory>
#include <string>
#include "../AUIWindow.hpp"

class NetworkManager;

namespace GUI {

class PlayerInfoWindow : public AUIWindow {
    public:
        explicit PlayerInfoWindow(std::shared_ptr<IGuiLib> guiLib);
        ~PlayerInfoWindow() = default;
        void setSelectedPlayer(int playerId);
        int getSelectedPlayer() const;
        bool hasPlayerSelected() const;
        void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);

    protected:
        void renderContent() override;
        void displayNoPlayerSelected();
        std::shared_ptr<const IPlayer> findSelectedPlayer();
        void displayPlayerNotFound();
        float displayPlayerIdentity(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        float displayPositionInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        float displayLevelInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        float displayInventory(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        std::string getOrientationString(int orientation);
        bool playerExistsInGameState(std::shared_ptr<const GameState> gameState);

        int _selectedPlayerId;
        bool _hasSelectedPlayer;
        std::shared_ptr<NetworkManager> _networkManager;
};

} // namespace GUI
