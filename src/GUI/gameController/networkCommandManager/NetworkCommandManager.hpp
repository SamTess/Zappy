/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** NetworkCommandManager
*/

#ifndef NETWORK_COMMAND_MANAGER_HPP_
#define NETWORK_COMMAND_MANAGER_HPP_

#include <memory>
#include <string>
#include "../../shared/commands/ICommandExecutor.hpp"
#include "../GameNetworkCommand.hpp"

class NetworkCommandManager {
    public:
        explicit NetworkCommandManager(std::shared_ptr<ICommandExecutor> commandExecutor);
        ~NetworkCommandManager() = default;

        void requestPlayerPosition(int playerId);
        void requestPlayerLevel(int playerId);
        void requestPlayerInventory(int playerId);
        void requestPlayerInfo(int playerId);
        void requestMapSize();
        void requestMapContent();
        void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor);

    private:
        std::shared_ptr<ICommandExecutor> _commandExecutor;
        void executeCommand(const std::string& command);
};

#endif /* !NETWORK_COMMAND_MANAGER_HPP_ */
