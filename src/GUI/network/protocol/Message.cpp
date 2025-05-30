/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** message
*/

#include "Message.hpp"
#include <utility>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

Message::Message() {
    std::cout << "creating empty message" << std::endl;
    _messageString = "";
    _messageHeader = "";
    _messageData = "";
    _structuredData = nullptr;
    _messageType = HeaderMessage::UNKNOWN;
}

Message::Message(const std::string& header, const std::string& data, std::shared_ptr<IMessageData> structuredData) {
    _messageHeader = header;
    _messageData = data;
    if (data[data.size() - 1] != '\n')
        _messageData += '\n';
    _messageString = _messageHeader + " " + _messageData;
    _structuredData = structuredData;
    _messageType = HeaderMessage::UNKNOWN;
}

void Message::setMessage(const std::string &data) {
    _messageString = data;
}

void Message::setHeader(const std::string &header) {
    _messageHeader = header;
}

void Message::setData(const std::string &data) {
    _messageData = data;
    _structuredData = nullptr;
}

void Message::setData(std::shared_ptr<IMessageData> data) {
    _structuredData = data;
}

const std::string &Message::getMessage() const {
    return _messageString;
}

const std::string &Message::getHeader() const {
    return _messageHeader;
}

const std::string &Message::getData() const {
    return _messageData;
}

bool Message::hasStructuredData() const {
    return _structuredData != nullptr;
}

HeaderMessage::MessageType Message::getType() const {
    return _messageType;
}

void Message::setType(HeaderMessage::MessageType type) {
    _messageType = type;
}

const std::vector<std::string> &Message::getParameters() const {
    return _parameters;
}

void Message::setParameters(const std::vector<std::string> &params) {
    _parameters = params;
}

int Message::getIntParam(size_t index) const {
    if (index >= _parameters.size())
        throw std::out_of_range("Index hors limites pour les paramètres du message");
    try {
        return std::stoi(_parameters[index]);
    } catch (const std::exception &e) {
        throw std::invalid_argument("Le paramètre n'est pas un entier valide");
    }
}
