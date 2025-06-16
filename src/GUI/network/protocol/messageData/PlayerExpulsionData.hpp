/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerExpulsionData
*/

#ifndef PLAYER_EXPULSION_DATA_HPP_
#define PLAYER_EXPULSION_DATA_HPP_

#include "IMessageData.hpp"

class PlayerExpulsionData : public IMessageData {
    public:
        PlayerExpulsionData(int playerId) : _playerId(playerId) {}

        MessageType getType() const override { return MessageType::PlayerExpulsion; }

        int getPlayerId() const { return _playerId; }
        void setPlayerId(int value) { _playerId = value; }

    private:
        int _playerId;
};

#endif /* !PLAYER_EXPULSION_DATA_HPP_ */
