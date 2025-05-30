/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EndGameData
*/

#ifndef END_GAME_DATA_HPP_
#define END_GAME_DATA_HPP_

#include "IMessageData.hpp"
#include <string>

class EndGameData : public IMessageData {
    public:
        explicit EndGameData(const std::string& teamName)
            : _teamName(teamName) {}

        MessageType getType() const override { return MessageType::EndGame; }

        const std::string& getTeamName() const { return _teamName; }

        void setTeamName(const std::string& value) { _teamName = value; }

    private:
        std::string _teamName;
};

#endif /* !END_GAME_DATA_HPP_ */
