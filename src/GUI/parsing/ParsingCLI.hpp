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
        ParsingCLI(bool testMode = false);
        ~ParsingCLI();

        void parse(int argc, char **argv);

        int getPort() const;
        const std::string &getMachine() const;

        void setPort(int port);
        void setMachine(const std::string &machine);

        void displayHelp() const;

        void checkValidityCLI() const;

    private:
        int _port;
        std::string _machine;
        bool _testMode;

        class ParsingCLIException : public AException {
            public:
                explicit ParsingCLIException(const std::string &message)
                : AException("ParsingCLIException", message) {}
        };
};

#endif /* !PARSINGCLI_HPP_ */
