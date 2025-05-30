/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ServerMessageData
*/

#ifndef SERVER_MESSAGE_DATA_HPP_
#define SERVER_MESSAGE_DATA_HPP_

#include <string>
#include "IMessageData.hpp"

class ServerMessageData : public IMessageData {
    public:
        explicit ServerMessageData(const std::string& message)
            : _message(message) {}

        MessageType getType() const override { return MessageType::ServerMessage; }

        const std::string& getMessage() const { return _message; }

        void setMessage(const std::string& value) { _message = value; }

    private:
        std::string _message;
};

#endif /* !SERVER_MESSAGE_DATA_HPP_ */
