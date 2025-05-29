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
}

Message::Message(std::string header, std::string data) {
    _messageHeader = std::move(header);
    _messageData = std::move(data);
    _messageString = _messageHeader + " " + _messageData;
}

void Message::setMessage(const std::string &data) {
    _messageString = data;
}

void Message::setHeader(const std::string &header) {
    _messageHeader = header;
}

void Message::setData(const std::string &data) {
    _messageData = data;
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
