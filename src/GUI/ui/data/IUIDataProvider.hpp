/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** UI data provider Interface
*/

#ifndef IUI_DATA_PROVIDER_HPP_
    #define IUI_DATA_PROVIDER_HPP_
    #include <memory>
    #include <vector>
    #include <string>
    #include <map>
    #include "../../gameController/IPlayer.hpp"
    #include "../../gameController/ITile.hpp"
    #include "../../gameController/IPlayerInventory.hpp"
    #include "../../gameController/IBroadcast.hpp"

namespace GUI {

class IUIDataProvider {
public:
    virtual ~IUIDataProvider() = default;

    virtual int getMapWidth() const = 0;
    virtual int getMapHeight() const = 0;
    virtual float getGameTime() const = 0;
    virtual int getFrequency() const = 0;
    virtual int getGameTick() const = 0;
    virtual bool isMapInitialized() const = 0;
    virtual bool isGameEnded() const = 0;
    virtual const std::string& getWinningTeam() const = 0;

    virtual std::shared_ptr<const ITile> getTile(int x, int y) const = 0;
    virtual int getResourceQuantity(int x, int y, ResourceType resourceType) const = 0;
    virtual ResourceType getDominantResourceType(int x, int y) const = 0;
    virtual std::vector<int> getPlayersOnTile(int x, int y) const = 0;
    virtual std::vector<int> getEggsOnTile(int x, int y) const = 0;

    virtual std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const = 0;
    virtual std::shared_ptr<const IPlayerInventory> getPlayerInventory(int playerId) const = 0;
    virtual bool isPlayerOnTile(int x, int y, int playerId) const = 0;
    virtual std::vector<int> getPlayerIds() const = 0;

    virtual const std::vector<std::string>& getTeamNames() const = 0;

    virtual std::vector<std::shared_ptr<const IBroadcast>> getBroadcasts() const = 0;

    virtual std::vector<int> calculateTotalResources() const = 0;
    virtual std::map<std::string, int> getTeamPlayerCounts() const = 0;
    virtual std::map<std::string, std::vector<int>> getTeamResourceTotals() const = 0;
};

} // namespace GUI

#endif /* IUI_DATA_PROVIDER_HPP_ */
