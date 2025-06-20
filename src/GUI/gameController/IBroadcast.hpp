/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IBroadcast - Interface pour les broadcasts
*/

#ifndef IBROADCAST_HPP_
#define IBROADCAST_HPP_

#include <string>

class IBroadcast {
public:
    virtual ~IBroadcast() = default;

    virtual const std::string& getTeam() const = 0;
    virtual const std::string& getMessage() const = 0;
    virtual float getTimeLeft() const = 0;
    virtual int getPlayerId() const = 0;

    virtual void setTeam(const std::string& team) = 0;
    virtual void setMessage(const std::string& message) = 0;
    virtual void setTimeLeft(float timeLeft) = 0;
    virtual void setPlayerId(int playerId) = 0;
    virtual void updateTimeLeft(float deltaTime) = 0;
};

#endif /* !IBROADCAST_HPP_ */
