/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ComponentCoordinator
*/

#include "ComponentCoordinator.hpp"
#include <iostream>

void ComponentCoordinator::setNetworkManager(std::shared_ptr<NetworkManager> networkManager) {
    _networkManager = networkManager;
}

void ComponentCoordinator::setGameController(std::shared_ptr<IGameController> gameController) {
    _gameController = gameController;
}

void ComponentCoordinator::setUINotifier(std::shared_ptr<IUINotifier> uiNotifier) {
    _uiNotifier = uiNotifier;
}

void ComponentCoordinator::executeCommand(std::shared_ptr<ICommand> command) {
    if (command && command->getDescription().length() > 0) {
        if (auto nm = _networkManager.lock())
            nm->sendCommand(command->getDescription());
    }
}

void ComponentCoordinator::setupConnections() {
    if (auto nm = _networkManager.lock()) {
        nm->setMessageHandler([this](const Message& message) {
            this->onMessageReceived(message);
        });
        nm->setConnectionCallback([this](bool connected) {
            (void)connected;
            // this->onConnectionStatusChanged(connected);
        });
    }
    if (auto gc = _gameController.lock())
        gc->setCommandExecutor(shared_from_this());
    if (auto ui = _uiNotifier.lock()) {
        if (auto nm = _networkManager.lock())
            ui->setCommandSender(nm);
        ui->setCommandExecutor(shared_from_this());
    }
}

void ComponentCoordinator::onMessageReceived(const Message& message) {
    if (auto gc = _gameController.lock())
        gc->processMessage(message);
}
