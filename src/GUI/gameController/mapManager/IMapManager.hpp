/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IMapManager interface
*/

#ifndef IMAP_MANAGER_HPP_
#define IMAP_MANAGER_HPP_

#include <memory>
#include <vector>
#include "../ITile.hpp"
#include "../IResource.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"

class IMapManager {
public:
    virtual ~IMapManager() = default;
    virtual int getMapWidth() const = 0;
    virtual int getMapHeight() const = 0;
    virtual bool isMapInitialized() const = 0;
    virtual void setMapSize(int width, int height) = 0;
    virtual std::shared_ptr<const ITile> getTile(int x, int y) const = 0;
    virtual std::shared_ptr<ITile> getTileMutable(int x, int y) = 0;
    virtual int getResourceQuantity(int x, int y, ResourceType resourceType) const = 0;
    virtual void updateTileResources(int x, int y, int food, int linemate, int deraumere,
        int sibur, int mendiane, int phiras, int thystame) = 0;
    virtual void setTileIncantationState(int x, int y, bool isIncantating) = 0;
    virtual std::vector<int> getPlayersOnTile(int x, int y) const = 0;
    virtual bool isPlayerOnTile(int x, int y, int playerId) const = 0;
    virtual void addPlayerToTile(int playerId, int x, int y) = 0;
    virtual void removePlayerFromTile(int playerId, int x, int y) = 0;
    virtual std::vector<int> getEggsOnTile(int x, int y) const = 0;
    virtual void addEggToTile(int eggId, int x, int y) = 0;
    virtual void removeEggFromTile(int eggId, int x, int y) = 0;
    virtual bool isValidCoordinates(int x, int y) const = 0;
};

#endif /* !IMAP_MANAGER_HPP_ */
