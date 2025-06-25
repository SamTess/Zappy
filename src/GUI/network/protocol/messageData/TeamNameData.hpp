/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TeamNameData
*/

#ifndef TEA_NAME_DATA_HPP_
#define TEA_NAME_DATA_HPP_

#include <string>
#include <vector>
#include "IMessageData.hpp"

class TeamNameData : public IMessageData {
    public:
        explicit TeamNameData(const std::vector<std::string>& names)
            : _names(names) {}

        MessageType getType() const override { return MessageType::TeamName; }

        const std::vector<std::string>& getNames() const { return _names; }

        void setNames(const std::vector<std::string>& value) { _names = value; }
        void addName(const std::string& name) { _names.push_back(name); }

    private:
        std::vector<std::string> _names;
};

#endif /* !TEA_NAME_DATA_HPP_ */
