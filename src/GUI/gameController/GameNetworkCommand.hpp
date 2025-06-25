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
    explicit GameNetworkCommand(const std::string& command) : m_command(command) {}

    void execute() override {};
    std::string getDescription() const override { return m_command; }
    const std::string& getCommand() const { return m_command; }

private:
    std::string m_command;
};


#endif /* !GAME_NETWORK_COMMAND_HPP_ */
