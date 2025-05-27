/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParsingCLI
*/

#include "ParsingCLI.hpp"
#include <iostream>
#include <cstring>

ParsingCLI::ParsingCLI(int argc, char **argv) {
    _port = 0;
    parse(argc, argv);
}

ParsingCLI::~ParsingCLI() {
}

int ParsingCLI::getPort() const {
    return _port;
}
const std::string &ParsingCLI::getMachine() const {
    return _machine;
}

void ParsingCLI::setPort(int port) {
    if (port < 1 || port > 65535)
        throw ParsingCLIException("Port number must be between 1 and 65535");
    _port = port;
}

void ParsingCLI::setMachine(const std::string &machine) {
    if (!machine.empty() && machine[0] == '-')
        throw ParsingCLIException("Invalid machine name starting with '-'");
    _machine = machine;
}

void ParsingCLI::parse(int argc, char **argv) {
    if (argc == 1) {
        displayHelp();
        throw ParsingCLIException("No arguments provided");
    }
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            setPort(std::stoi(argv[++i]));
        } else if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            setMachine(argv[++i]);
        } else {
            displayHelp();
            throw ParsingCLIException("Invalid argument: " + std::string(argv[i]));
        }
    }
    checkValidityCLI();
}

void ParsingCLI::checkValidityCLI() const {
    if (_machine.empty())
        throw ParsingCLIException("Machine name cannot be empty");
    if (_port <= 0)
        throw ParsingCLIException("Port number must be set");
}

void ParsingCLI::displayHelp() const {
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
    std::cout << "\t-p port      port number" << std::endl;
    std::cout << "\t-h machine   hostname of the server" << std::endl;
}