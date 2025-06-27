/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** ICommandExecutor
*/

#ifndef ICOMMAND_EXECUTOR_HPP_
    #define ICOMMAND_EXECUTOR_HPP_
    #include <memory>
    #include <string>
    #include "ICommand.hpp"

class ICommandExecutor {
public:
    virtual ~ICommandExecutor() = default;
    virtual void executeCommand(std::shared_ptr<ICommand> command) = 0;
};

#endif /* !ICOMMAND_EXECUTOR_HPP_ */
