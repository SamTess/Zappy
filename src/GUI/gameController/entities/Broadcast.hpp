/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Broadcast - Implémentation concrète d'un broadcast
*/

#ifndef BROADCAST_HPP_
#define BROADCAST_HPP_

#include <string>
#include "../IBroadcast.hpp"

class Broadcast : public IBroadcast {
public:
    Broadcast() = default;
    explicit Broadcast(const std::string& team, const std::string& message, int playerId, float timeLeft = 15.0f)
        : _team(team), _message(message), _timeLeft(timeLeft), _playerId(playerId) {}

    const std::string& getTeam() const override { return _team; }
    const std::string& getMessage() const override { return _message; }
    float getTimeLeft() const override { return _timeLeft; }
    int getPlayerId() const override { return _playerId; }

    void setTeam(const std::string& team) override { _team = team; }
    void setMessage(const std::string& message) override { _message = message; }
    void setTimeLeft(float timeLeft) override { _timeLeft = timeLeft; }
    void setPlayerId(int playerId) override { _playerId = playerId; }

    void updateTimeLeft(float deltaTime) override {
        _timeLeft -= deltaTime;
        if (_timeLeft < 0.0f)
            _timeLeft = 0.0f;
    }

private:
    std::string _team;
    std::string _message;
    float _timeLeft = 15.0f;
    int _playerId = 0;
};

#endif /* !BROADCAST_HPP_ */
