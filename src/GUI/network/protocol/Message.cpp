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

Message::Message() {
    std::cout << "creating empty message" << std::endl;
    _messageString = "";
    _messageHeader = "";
    _messageData = "";
    _structuredData = nullptr;
}

Message::Message(const std::string& header, const std::string& data, std::shared_ptr<IMessageData> structuredData) {
    _messageHeader = header;
    _messageData = data;
    if (data[data.size() - 1] != '\n')
        _messageData += '\n';
    _messageString = _messageHeader + " " + _messageData;
    _structuredData = structuredData;
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
