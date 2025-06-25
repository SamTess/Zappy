/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ComponentCoordinator implementation
*/

#include "ComponentCoordinator.hpp"
#include <iostream>

void ComponentCoordinator::setNetworkManager(std::shared_ptr<NetworkManager> networkManager) {
    m_networkManager = networkManager;
}

void ComponentCoordinator::setGameController(std::shared_ptr<GameController> gameController) {
    m_gameController = gameController;
}

void ComponentCoordinator::setUINotifier(std::shared_ptr<IUINotifier> uiNotifier) {
    m_uiNotifier = uiNotifier;
}

void ComponentCoordinator::executeCommand(std::shared_ptr<ICommand> command) {
    if (command && command->getDescription().length() > 0) {
        if (auto nm = m_networkManager.lock())
            nm->sendCommand(command->getDescription());
    }
}

void ComponentCoordinator::setupConnections() {
    if (auto nm = m_networkManager.lock()) {
        nm->setMessageHandler([this](const Message& message) {
            this->onMessageReceived(message);
        });
        nm->setConnectionCallback([this](bool connected) {
            this->onConnectionStatusChanged(connected);
        });
    }
    if (auto gc = m_gameController.lock())
        gc->setCommandExecutor(shared_from_this());
    if (auto ui = m_uiNotifier.lock()) {
        if (auto nm = m_networkManager.lock())
            ui->setCommandSender(nm);
    }
}

void ComponentCoordinator::onMessageReceived(const Message& message) {
    if (auto gc = m_gameController.lock())
        gc->onMessageReceived(message);
}

void ComponentCoordinator::onConnectionStatusChanged(bool connected) {
    if (auto ui = m_uiNotifier.lock())
        ui->notifyConnectionStatus(connected);
}
