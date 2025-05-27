/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParsingCLI
*/

#ifndef PARSINGCLI_HPP_
#define PARSINGCLI_HPP_

#include <string>
#include "../shared/exception/AException.hpp"

class ParsingCLI {
    public:
        ParsingCLI(int argc, char **argv);
        ~ParsingCLI();

        void parse(int argc, char **argv);

        int getPort() const;
        const std::string &getMachine() const;

        void setPort(int port);
        void setMachine(const std::string &machine);

        void displayHelp() const;

        void checkValidityCLI() const;

    protected:
    private:
        int _port;
        std::string _machine;

        class ParsingCLIException : public AException {
            public:
                ParsingCLIException(const std::string &message)
                : AException("ParsingCLIException", message) {}
        };
};

#endif /* !PARSINGCLI_HPP_ */
