/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Network Command implementation
*/

#ifndef GAME_NETWORK_COMMAND_HPP_
    #define GAME_NETWORK_COMMAND_HPP_
    #include <string>
    #include "../shared/commands/ICommand.hpp"

class GameNetworkCommand : public ICommand {
public:
    explicit GameNetworkCommand(const std::string& command) : _command(command) {}

    void execute() override {};
    std::string getDescription() const override { return _command; }
    const std::string& getCommand() const { return _command; }

private:
    std::string _command;
};


#endif /* !GAME_NETWORK_COMMAND_HPP_ */
