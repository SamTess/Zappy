/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameState - Store passif des données de jeu
*/

#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <array>
#include <mutex>

#include "../network/protocol/messageData/MessageDataAll.hpp"


struct TileData {
    std::array<int, 7> resources{0};
    std::vector<int> playerIds;
    std::vector<int> eggIds;
    bool isIncantating{false};
};


enum class ResourceType {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    COUNT
};

class GameState {
public:
    GameState();
    ~GameState() = default;
    int getMapWidth() const;
    int getMapHeight() const;
    bool isMapInitialized() const;
    const TileData& getTileData(int x, int y) const;
    int getResourceQuantity(int x, int y, ResourceType resourceType) const;
    ResourceType getDominantResourceType(int x, int y) const;
    std::shared_ptr<const PlayerInfoData> getPlayerInfo(int playerId) const;
    std::shared_ptr<const PlayerInventoryData> getPlayerInventory(int playerId) const;
    bool isPlayerOnTile(int x, int y, int playerId) const;
    std::vector<int> getPlayersOnTile(int x, int y) const;
    std::shared_ptr<const EggData> getEggInfo(int eggId) const;
    std::vector<int> getEggsOnTile(int x, int y) const;
    const std::vector<std::string>& getTeamNames() const;
    int getTimeUnit() const;
    bool isGameEnded() const;
    const std::string& getWinningTeam() const;
    void setMapSize(int width, int height);
    void updateTileResources(int x, int y, int food, int linemate, int deraumere,
                            int sibur, int mendiane, int phiras, int thystame);
    void setTileIncantationState(int x, int y, bool isIncantating);
    void addOrUpdatePlayer(const PlayerInfoData& playerData);
    void removePlayer(int playerId);
    void updatePlayerInventory(const PlayerInventoryData& inventoryData);
    void movePlayer(int playerId, int newX, int newY);
    void addEgg(const EggData& eggData);
    void removeEgg(int eggId);
    void setTeamNames(const std::vector<std::string>& teamNames);
    void setTimeUnit(int timeUnit);
    void setGameEnded(bool ended, const std::string& winningTeam = "");

private:
    bool isValidCoordinates(int x, int y) const;
    void addPlayerToTile(int playerId, int x, int y);
    void removePlayerFromTile(int playerId, int x, int y);
    void addEggToTile(int eggId, int x, int y);
    void removeEggFromTile(int eggId, int x, int y);
    mutable std::mutex _mutex;
    int _mapWidth = 0;
    int _mapHeight = 0;
    bool _isMapInitialized = false;
    std::vector<std::vector<TileData>> _mapTiles;
    std::map<int, PlayerInfoData> _players;
    std::map<int, PlayerInventoryData> _inventories;
    std::map<int, EggData> _eggs;
    std::vector<std::string> _teamNames;
    int _timeUnit = 100;
    bool _gameEnded = false;
    std::string _winningTeam;
};

#endif /* !GAME_STATE_HPP_ */
