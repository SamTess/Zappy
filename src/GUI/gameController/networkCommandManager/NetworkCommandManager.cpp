/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** NetworkCommandManager
*/

#include "NetworkCommandManager.hpp"
#include <iostream>

NetworkCommandManager::NetworkCommandManager(std::shared_ptr<ICommandExecutor> commandExecutor)
    : _commandExecutor(commandExecutor) {}

void NetworkCommandManager::requestPlayerPosition(int playerId) {
    executeCommand("ppo #" + std::to_string(playerId));
}

void NetworkCommandManager::requestPlayerLevel(int playerId) {
    executeCommand("plv #" + std::to_string(playerId));
}

void NetworkCommandManager::requestPlayerInventory(int playerId) {
    executeCommand("pin #" + std::to_string(playerId));
}

void NetworkCommandManager::requestPlayerInfo(int playerId) {
    requestPlayerPosition(playerId);
    requestPlayerLevel(playerId);
    requestPlayerInventory(playerId);
}

void NetworkCommandManager::requestMapSize() {
    executeCommand("msz\n");
}

void NetworkCommandManager::requestMapContent() {
    executeCommand("mct\n");
}

void NetworkCommandManager::setCommandExecutor(std::shared_ptr<ICommandExecutor> executor) {
    _commandExecutor = executor;
}

void NetworkCommandManager::executeCommand(const std::string& command) {
    if (!_commandExecutor) {
        std::cerr << "[NetworkCommandManager] No command executor set" << std::endl;
        return;
    }
    auto networkCommand = std::make_shared<GameNetworkCommand>(command);
    _commandExecutor->executeCommand(networkCommand);
}
