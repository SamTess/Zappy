/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Server Frequency Command header
*/

#ifndef SERVER_FREQUENCY_COMMAND_HPP_
#define SERVER_FREQUENCY_COMMAND_HPP_
#include <sstream>
#include <string>
#include "../../../shared/commands/ICommand.hpp"

class ServerFrequencyCommand : public ICommand {
public:
    explicit ServerFrequencyCommand(int frequency) : m_frequency(frequency) {}
    void execute() override {}
    std::string getDescription() const override {
        std::stringstream ss;
        ss << "sst " << m_frequency;
        return ss.str();
    }

private:
    int m_frequency;
};

#endif /* !SERVER_FREQUENCY_COMMAND_HPP_ */
