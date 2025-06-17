/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ITile
*/

#ifndef ITILE_HPP_
#define ITILE_HPP_

#include "IGameEntity.hpp"
#include <string>

class TileContentData;

class ITile : public IRenderable {
public:
    virtual ~ITile() = default;
    virtual int getX() const = 0;
    virtual int getY() const = 0;
    virtual const std::array<int, 7>& getResources() const = 0;
    virtual int getResourceQuantity(ResourceType type) const = 0;
    virtual void setResourceQuantity(ResourceType type, int quantity) = 0;
    virtual ResourceType getDominantResourceType() const = 0;
    virtual const std::vector<int>& getPlayerIds() const = 0;
    virtual const std::vector<int>& getEggIds() const = 0;
    virtual bool isIncantating() const = 0;
    virtual void setIncantating(bool incantating) = 0;
    virtual void addPlayer(int playerId) = 0;
    virtual void removePlayer(int playerId) = 0;
    virtual void addEgg(int eggId) = 0;
    virtual void removeEgg(int eggId) = 0;
    virtual void updateFromProtocol(const TileContentData& data) = 0;
};

#endif /* !ITILE_HPP_ */
