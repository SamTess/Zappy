/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameState
*/

#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <array>
#include <mutex>
#include <deque>
#include "IBroadcast.hpp"
#include "IGameEntity.hpp"
#include "EntitiesAll.hpp"
#include "EntityFactory.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"


class GameState {
public:
    GameState();
    explicit GameState(std::shared_ptr<EntityFactoryManager> factory);
    ~GameState() = default;
    int getMapWidth() const;
    int getMapHeight() const;
    bool isMapInitialized() const;
    std::shared_ptr<const ITile> getTile(int x, int y) const;
    std::shared_ptr<ITile> getTileMutable(int x, int y);
    int getResourceQuantity(int x, int y, ResourceType resourceType) const;
    std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const;
    std::shared_ptr<const IPlayerInventory> getPlayerInventory(int playerId) const;
    bool isPlayerOnTile(int x, int y, int playerId) const;
    std::vector<int> getPlayersOnTile(int x, int y) const;
    std::shared_ptr<const IEgg> getEggInfo(int eggId) const;
    std::vector<int> getEggsOnTile(int x, int y) const;
    const std::vector<std::string>& getTeamNames() const;
    int getTimeUnit() const;
    bool isGameEnded() const;
    const std::string& getWinningTeam() const;
    std::vector<int> getPlayerIds() const;
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
    std::map<int, std::shared_ptr<IPlayer>> getPlayers();
    const std::map<int, std::shared_ptr<IPlayer>>& getPlayers() const;
    bool getSfxEnabled() const;
    void setSfxEnabled(bool enabled);
    float getMusicVolume() const;
    void setMusicVolume(float volume);

private:
    bool isValidCoordinates(int x, int y) const;
    void addPlayerToTile(int playerId, int x, int y);
    void removePlayerFromTile(int playerId, int x, int y);
    void addEggToTile(int eggId, int x, int y);
    void removeEggFromTile(int eggId, int x, int y);

    int _mapWidth = 0;
    int _mapHeight = 0;
    bool _isMapInitialized = false;
    std::vector<std::vector<std::shared_ptr<ITile>>> _tiles;
    std::map<int, std::shared_ptr<IPlayer>> _players;
    std::map<int, std::shared_ptr<IPlayerInventory>> _inventories;
    std::map<int, std::shared_ptr<IEgg>> _eggs;
    std::vector<std::string> _teamNames;
    int _timeUnit = 100;
    bool _gameEnded = false;
    bool _sfxEnabled = true;
    float _musicVolume = 0.8f;
    std::string _winningTeam;
    std::shared_ptr<EntityFactoryManager> _entityFactory;
};

#endif /* !GAME_STATE_HPP_ */
