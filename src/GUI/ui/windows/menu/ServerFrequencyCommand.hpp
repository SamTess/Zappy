/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** ServerFrequencyCommand
*/

#ifndef SERVER_FREQUENCY_COMMAND_HPP_
#define SERVER_FREQUENCY_COMMAND_HPP_
#include <sstream>
#include <string>
#include "../../../shared/commands/ICommand.hpp"

class ServerFrequencyCommand : public ICommand {
    public:
        explicit ServerFrequencyCommand(int frequency) : _frequency(frequency) {}
        void execute() override {}
        std::string getDescription() const override {
            std::stringstream ss;
            ss << "sst " << _frequency;
            return ss.str();
        }

    private:
        int _frequency;
};

#endif /* !SERVER_FREQUENCY_COMMAND_HPP_ */
