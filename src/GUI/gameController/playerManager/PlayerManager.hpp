/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerManager
*/

#ifndef PLAYER_MANAGER_HPP_
#define PLAYER_MANAGER_HPP_

#include <map>
#include <vector>
#include <memory>
#include "IPlayerManager.hpp"
#include "../EntityFactory.hpp"
#include "../entities/Player.hpp"
#include "../entities/PlayerInventory.hpp"
#include "../entities/Egg.hpp"

class IMapManager;

class PlayerManager : public IPlayerManager {
public:
    PlayerManager();
    explicit PlayerManager(std::shared_ptr<EntityFactoryManager> factory);
    ~PlayerManager() = default;

    void setMapManager(std::shared_ptr<IMapManager> mapManager);
    std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const override;
    std::vector<int> getPlayerIds() const override;
    const std::map<int, std::shared_ptr<IPlayer>> getPlayers() const override;
    void addOrUpdatePlayer(const PlayerInfoData& playerData) override;
    void removePlayer(int playerId) override;
    void movePlayer(int playerId, int newX, int newY) override;
    std::shared_ptr<const IPlayerInventory> getPlayerInventory(int playerId) const override;
    void updatePlayerInventory(const PlayerInventoryData& inventoryData) override;
    std::shared_ptr<const IEgg> getEggInfo(int eggId) const override;
    void addEgg(const EggData& eggData) override;
    void removeEgg(int eggId) override;

private:
    std::map<int, std::shared_ptr<IPlayer>> _players;
    std::map<int, std::shared_ptr<IPlayerInventory>> _inventories;
    std::map<int, std::shared_ptr<IEgg>> _eggs;
    std::shared_ptr<EntityFactoryManager> _entityFactory;
    std::shared_ptr<IMapManager> _mapManager;
};

#endif /* !PLAYER_MANAGER_HPP_ */
