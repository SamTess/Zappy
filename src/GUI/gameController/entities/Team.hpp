/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Team - Implémentation concrète d'une équipe
*/

#ifndef TEAM_HPP_
#define TEAM_HPP_

#include <string>
#include "../ITeam.hpp"

class Team : public ITeam {
public:
    Team() = default;
    explicit Team(const std::string& name, int remainingSlots = 0)
        : _name(name), _remainingSlots(remainingSlots) {}

    const std::string& getName() const override { return _name; }
    int getRemainingSlots() const override { return _remainingSlots; }

    void setName(const std::string& name) override { _name = name; }
    void setRemainingSlots(int slots) override { _remainingSlots = slots; }

private:
    std::string _name;
    int _remainingSlots = 0;
};

#endif /* !TEAM_HPP_ */
