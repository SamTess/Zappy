/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameStatusManager
*/

#include <vector>
#include <string>
#include "GameStatusManager.hpp"

GameStatusManager::GameStatusManager()
    : _timeUnit(100), _gameEnded(false) {
}

const std::vector<std::string>& GameStatusManager::getTeamNames() const {
    return _teamNames;
}

void GameStatusManager::setTeamNames(const std::vector<std::string>& teamNames) {
    _teamNames.insert(_teamNames.end(), teamNames.begin(), teamNames.end());
}

int GameStatusManager::getTimeUnit() const {
    return _timeUnit;
}

void GameStatusManager::setTimeUnit(int timeUnit) {
    _timeUnit = timeUnit;
}

bool GameStatusManager::isGameEnded() const {
    return _gameEnded;
}

const std::string& GameStatusManager::getWinningTeam() const {
    return _winningTeam;
}

void GameStatusManager::setGameEnded(bool ended, const std::string& winningTeam) {
    _gameEnded = ended;
    _winningTeam = winningTeam;
}
