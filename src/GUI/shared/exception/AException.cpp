/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** AException
*/

#include <string>
#include "AException.hpp"

AException::AException(const std::string &type, const std::string &message)
    : _message(message), _type(type) {
}

std::string AException::getMessage() const noexcept {
        return _message;
}

std::string AException::getType() const noexcept {
    return _type;
}

std::string AException::getFormattedMessage() const noexcept {
    return "[" + _type + "] " + _message;
}

const char* AException::what() const noexcept {
    return getFormattedMessage().c_str();
}

