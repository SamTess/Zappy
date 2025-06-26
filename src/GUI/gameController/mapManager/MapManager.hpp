/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MapManager
*/

#ifndef MAP_MANAGER_HPP_
#define MAP_MANAGER_HPP_

#include <vector>
#include <memory>
#include "IMapManager.hpp"
#include "../EntityFactory.hpp"
#include "../entities/Tile.hpp"

class MapManager : public IMapManager {
public:
    MapManager();
    explicit MapManager(std::shared_ptr<EntityFactoryManager> factory);
    ~MapManager() = default;
    int getMapWidth() const override;
    int getMapHeight() const override;
    bool isMapInitialized() const override;
    void setMapSize(int width, int height) override;
    std::shared_ptr<const ITile> getTile(int x, int y) const override;
    std::shared_ptr<ITile> getTileMutable(int x, int y) override;
    int getResourceQuantity(int x, int y, ResourceType resourceType) const override;
    void updateTileResources(int x, int y, int food, int linemate, int deraumere,
        int sibur, int mendiane, int phiras, int thystame) override;
    void setTileIncantationState(int x, int y, bool isIncantating) override;
    std::vector<int> getPlayersOnTile(int x, int y) const override;
    bool isPlayerOnTile(int x, int y, int playerId) const override;
    void addPlayerToTile(int playerId, int x, int y) override;
    void removePlayerFromTile(int playerId, int x, int y) override;
    std::vector<int> getEggsOnTile(int x, int y) const override;
    void addEggToTile(int eggId, int x, int y) override;
    void removeEggFromTile(int eggId, int x, int y) override;
    bool isValidCoordinates(int x, int y) const override;

private:
    int _mapWidth;
    int _mapHeight;
    bool _isMapInitialized;
    std::vector<std::vector<std::shared_ptr<ITile>>> _tiles;
    std::shared_ptr<EntityFactoryManager> _entityFactory;
};

#endif /* !MAP_MANAGER_HPP_ */
