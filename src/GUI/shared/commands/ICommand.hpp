/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ICommand
*/

#ifndef ICOMMAND_HPP_
#define ICOMMAND_HPP_

#include <memory>
#include <string>

class ICommand {
    public:
        virtual ~ICommand() = default;
        virtual void execute() = 0;
        virtual std::string getDescription() const = 0;
};

#endif /* !ICOMMAND_HPP_ */
