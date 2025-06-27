/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameStatusManager
*/

#ifndef GAME_STATUS_MANAGER_HPP_
#define GAME_STATUS_MANAGER_HPP_

#include <vector>
#include <string>
#include "IGameStatusManager.hpp"

class GameStatusManager : public IGameStatusManager {
public:
    GameStatusManager();
    ~GameStatusManager() = default;

    const std::vector<std::string>& getTeamNames() const override;
    void setTeamNames(const std::vector<std::string>& teamNames) override;
    int getTimeUnit() const override;
    void setTimeUnit(int timeUnit) override;
    bool isGameEnded() const override;
    const std::string& getWinningTeam() const override;
    void setGameEnded(bool ended, const std::string& winningTeam = "") override;

private:
    std::vector<std::string> _teamNames;
    int _timeUnit;
    bool _gameEnded;
    std::string _winningTeam;
};

#endif /* !GAME_STATUS_MANAGER_HPP_ */
