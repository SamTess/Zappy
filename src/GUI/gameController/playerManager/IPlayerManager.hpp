/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IPlayerManager interface
*/

#ifndef IPLAYER_MANAGER_HPP_
#define IPLAYER_MANAGER_HPP_

#include <memory>
#include <map>
#include <vector>
#include "../IPlayer.hpp"
#include "../IPlayerInventory.hpp"
#include "../IEgg.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"

class IPlayerManager {
public:
    virtual ~IPlayerManager() = default;

    // Player management
    virtual std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const = 0;
    virtual std::vector<int> getPlayerIds() const = 0;
    virtual const std::map<int, std::shared_ptr<IPlayer>> getPlayers() const = 0;
    virtual void addOrUpdatePlayer(const PlayerInfoData& playerData) = 0;
    virtual void removePlayer(int playerId) = 0;
    virtual void movePlayer(int playerId, int newX, int newY) = 0;

    // Player inventory management
    virtual std::shared_ptr<const IPlayerInventory> getPlayerInventory(int playerId) const = 0;
    virtual void updatePlayerInventory(const PlayerInventoryData& inventoryData) = 0;

    // Egg management
    virtual std::shared_ptr<const IEgg> getEggInfo(int eggId) const = 0;
    virtual void addEgg(const EggData& eggData) = 0;
    virtual void removeEgg(int eggId) = 0;
};

#endif /* !IPLAYER_MANAGER_HPP_ */
