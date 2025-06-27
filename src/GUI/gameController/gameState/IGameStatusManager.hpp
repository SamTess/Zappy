/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IGameStatusManager
*/

#ifndef IGAME_STATUS_MANAGER_HPP_
#define IGAME_STATUS_MANAGER_HPP_

#include <string>
#include <vector>

class IGameStatusManager {
public:
    virtual ~IGameStatusManager() = default;

    virtual const std::vector<std::string>& getTeamNames() const = 0;
    virtual void setTeamNames(const std::vector<std::string>& teamNames) = 0;
    virtual int getTimeUnit() const = 0;
    virtual void setTimeUnit(int timeUnit) = 0;
    virtual bool isGameEnded() const = 0;
    virtual const std::string& getWinningTeam() const = 0;
    virtual void setGameEnded(bool ended, const std::string& winningTeam = "") = 0;
};

#endif /* !IGAME_STATUS_MANAGER_HPP_ */
