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
#include "../../../shared/commands/ICommand.hpp"
#include "../../../shared/commands/INetworkCommandSender.hpp"

class NetworkManager;

namespace GUI {

class PlayerInfoWindow : public AUIWindow {
    public:
        explicit PlayerInfoWindow(std::shared_ptr<IGuiLib> guiLib);
        ~PlayerInfoWindow() = default;

        void setSelectedPlayer(int playerId);
        int getSelectedPlayer() const;
        bool hasPlayerSelected() const;
        void setCommandSender(std::shared_ptr<INetworkCommandSender> sender);

    protected:
        void renderContent() override;

    private:
        void displayNoPlayerSelected();
        std::shared_ptr<const IPlayer> findSelectedPlayer();
        void displayPlayerNotFound();
        float displayPlayerIdentity(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        float displayPositionInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        float displayLevelInfo(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        float displayInventory(std::shared_ptr<const IPlayer> selectedPlayer, float yOffset);
        std::string getOrientationString(int orientation);
        bool playerExistsInDataProvider();
        int _selectedPlayerId;
        bool _hasSelectedPlayer;
        std::shared_ptr<INetworkCommandSender> _commandSender;
};

} // namespace GUI
