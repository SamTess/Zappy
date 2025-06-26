/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameStateAdapter
*/

#ifndef GAME_STATE_ADAPTER_HPP_
#define GAME_STATE_ADAPTER_HPP_

#include <memory>
#include <vector>
#include <map>
#include <string>
#include "gameState/IGameStateManager.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"
#include "IPlayer.hpp"
#include "IPlayerInventory.hpp"
#include "IEgg.hpp"
#include "ITile.hpp"
#include "IRenderable.hpp"

class GameStateAdapter {
    public:
        explicit GameStateAdapter(std::shared_ptr<IGameStateManager> gameStateManager);
        ~GameStateAdapter() = default;
        int getMapWidth() const;
        int getMapHeight() const;
        bool isMapInitialized() const;
        std::shared_ptr<const ITile> getTile(int x, int y) const;
        std::shared_ptr<ITile> getTileMutable(int x, int y);
        int getResourceQuantity(int x, int y, ResourceType resourceType) const;
        void setMapSize(int width, int height);
        void updateTileResources(int x, int y, int food, int linemate, int deraumere,
                                int sibur, int mendiane, int phiras, int thystame);
        void setTileIncantationState(int x, int y, bool isIncantating);
        bool isPlayerOnTile(int x, int y, int playerId) const;
        std::vector<int> getPlayersOnTile(int x, int y) const;
        std::vector<int> getEggsOnTile(int x, int y) const;
        std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const;
        std::shared_ptr<const IPlayerInventory> getPlayerInventory(int playerId) const;
        std::vector<int> getPlayerIds() const;
        const std::map<int, std::shared_ptr<IPlayer>> getPlayers() const;
        void addOrUpdatePlayer(const PlayerInfoData& playerData);
        void removePlayer(int playerId);
        void updatePlayerInventory(const PlayerInventoryData& inventoryData);
        void movePlayer(int playerId, int newX, int newY);
        std::shared_ptr<const IEgg> getEggInfo(int eggId) const;
        void addEgg(const EggData& eggData);
        void removeEgg(int eggId);
        const std::vector<std::string>& getTeamNames() const;
        void setTeamNames(const std::vector<std::string>& teamNames);
        int getTimeUnit() const;
        void setTimeUnit(int timeUnit);
        bool isGameEnded() const;
        const std::string& getWinningTeam() const;
        void setGameEnded(bool ended, const std::string& winningTeam = "");
        bool getSfxEnabled() const;
        void setSfxEnabled(bool enabled);
        float getMusicVolume() const;
        void setMusicVolume(float volume);
        std::shared_ptr<IGameStateManager> getGameStateManager() const;

    private:
        std::shared_ptr<IGameStateManager> _gameStateManager;
};

#endif /* !GAME_STATE_ADAPTER_HPP_ */
