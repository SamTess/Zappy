/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IPlayer
*/

#ifndef IPLAYER_HPP_
#define IPLAYER_HPP_

#include <string>
#include <memory>
#include "IGameEntity.hpp"

class PlayerInfoData;

class IPlayer : public IGameEntity {
public:
    virtual int getLevel() const = 0;
    virtual int getOrientation() const = 0;
    virtual const std::string& getTeamName() const = 0;
    virtual bool isAlive() const = 0;
    virtual void updateFromProtocol(const PlayerInfoData& data) = 0;
    virtual void setLevel(int level) = 0;
    virtual void setOrientation(int orientation) = 0;
    virtual void setTeamName(const std::string& teamName) = 0;
    virtual void setIsAlive(bool alive) = 0;

    virtual void renderPlayer(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        const ZappyTypes::Vector3& position,
        float tileSize,
        int playerIndex = 0,
        int totalPlayers = 1) const = 0;
};

#endif /* !IPLAYER_HPP_ */
