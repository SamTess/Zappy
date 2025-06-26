/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IUINotifier
*/

#ifndef IUINOTIFIER_HPP_
    #define IUINOTIFIER_HPP_
    #include <memory>
    #include <functional>
    #include "../../gameController/IGameController.hpp"
    #include "../../network/networkManager/NetworkManager.hpp"
    #include "../commands/ICommand.hpp"
    #include "../commands/ICommandExecutor.hpp"
    #include "../commands/INetworkCommandSender.hpp"

class IUINotifier {
public:
    virtual ~IUINotifier() = default;
    virtual void notifyConnectionStatus(bool connected) = 0;
    virtual void setCommandSender(std::shared_ptr<INetworkCommandSender> sender) = 0;
    virtual void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor) = 0;
};

#endif /* IUINOTIFIER_HPP_ */
