/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IncantationData
*/

#ifndef INCANTATION_DATA_HPP_
#define INCANTATION_DATA_HPP_

#include "IMessageData.hpp"
#include <vector>

class IncantationData : public IMessageData {
    public:
        IncantationData(int x, int y, int level, const std::vector<int>& playerIds)
            : _x(x), _y(y), _level(level), _playerIds(playerIds) {}
        MessageType getType() const override { return MessageType::Incantation; }

        int getX() const { return _x; }
        int getY() const { return _y; }
        int getLevel() const { return _level; }
        const std::vector<int>& getPlayerIds() const { return _playerIds; }

        void setX(int value) { _x = value; }
        void setY(int value) { _y = value; }
        void setLevel(int value) { _level = value; }
        void setPlayerIds(const std::vector<int>& value) { _playerIds = value; }
        void addPlayerId(int id) { _playerIds.push_back(id); }

    private:
        int _x;
        int _y;
        int _level;
        std::vector<int> _playerIds;
};

#endif /* !INCANTATION_DATA_HPP_ */
