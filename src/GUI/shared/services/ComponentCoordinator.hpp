/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ComponentCoordinator
*/

#ifndef COMPONENT_COORDINATOR_HPP_
#define COMPONENT_COORDINATOR_HPP_

#include <memory>
#include <functional>
#include "../../gameController/IGameController.hpp"
#include "../../network/networkManager/NetworkManager.hpp"
#include "../commands/ICommand.hpp"
#include "../commands/ICommandExecutor.hpp"
#include "../commands/INetworkCommandSender.hpp"
#include "IUINotifier.hpp"

class ComponentCoordinator : public ICommandExecutor, public std::enable_shared_from_this<ComponentCoordinator> {
public:
    ComponentCoordinator() = default;
    ~ComponentCoordinator() = default;

    void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);
    void setGameController(std::shared_ptr<IGameController> gameController);
    void setUINotifier(std::shared_ptr<IUINotifier> uiNotifier);
    void executeCommand(std::shared_ptr<ICommand> command);
    void setupConnections();

private:
    std::weak_ptr<NetworkManager> _networkManager;
    std::weak_ptr<IGameController> _gameController;
    std::weak_ptr<IUINotifier> _uiNotifier;

    void onMessageReceived(const Message& message);
    void onConnectionStatusChanged(bool connected);
};

#endif /* !COMPONENT_COORDINATOR_HPP_ */
