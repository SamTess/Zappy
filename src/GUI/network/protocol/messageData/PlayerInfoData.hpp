/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInfoData
*/

#ifndef PLAYER_INFO_DATA_HPP_
#define PLAYER_INFO_DATA_HPP_

#include "IMessageData.hpp"
#include <string>

class PlayerInfoData : public IMessageData {
    public:
        PlayerInfoData() : _id(0), _x(0), _y(0), _orientation(0), _level(0), _teamName("") {}
        PlayerInfoData(int id, int x, int y, int orientation, int level, const std::string& teamName = "")
            : _id(id), _x(x), _y(y), _orientation(orientation), _level(level), _teamName(teamName) {}
        MessageType getType() const override { return MessageType::PlayerInfo; }

        int getId() const { return _id; }
        int getX() const { return _x; }
        int getY() const { return _y; }
        int getOrientation() const { return _orientation; }
        int getLevel() const { return _level; }
        const std::string& getTeamName() const { return _teamName; }

        void setId(int value) { _id = value; }
        void setX(int value) { _x = value; }
        void setY(int value) { _y = value; }
        void setOrientation(int value) { _orientation = value; }
        void setLevel(int value) { _level = value; }
        void setTeamName(const std::string& value) { _teamName = value; }

    private:
        int _id;
        int _x;
        int _y;
        int _orientation;
        int _level;
        std::string _teamName;
};

#endif /* !PLAYER_INFO_DATA_HPP_ */
