/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BroadcastData
*/

#ifndef BROADCAST_DATA_HPP_
#define BROADCAST_DATA_HPP_

#include <string>
#include "IMessageData.hpp"

class BroadcastData : public IMessageData {
    public:
        BroadcastData(int playerId, const std::string& message)
            : _playerId(playerId), _message(message) {}
        MessageType getType() const override { return MessageType::Broadcast; }

        int getPlayerId() const { return _playerId; }
        const std::string& getMessage() const { return _message; }

        void setPlayerId(int value) { _playerId = value; }
        void setMessage(const std::string& value) { _message = value; }

    private:
        int _playerId;
        std::string _message;
};

#endif /* !BROADCAST_DATA_HPP_ */
