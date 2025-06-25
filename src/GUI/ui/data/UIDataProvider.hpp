/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** UIDataProvider implementation header
*/

#ifndef UI_DATA_PROVIDER_HPP_
    #define UI_DATA_PROVIDER_HPP_
    #include <deque>
    #include <vector>
    #include <memory>
    #include <string>
    #include <map>
    #include "IUIDataProvider.hpp"
    #include "../../gameController/GameState.hpp"

namespace GUI {

class UIDataProvider : public IUIDataProvider {
public:
    explicit UIDataProvider(std::shared_ptr<const GameState> gameState);
    virtual ~UIDataProvider() = default;

    void updateGameState(std::shared_ptr<const GameState> gameState);
    void updateTimeData(float gameTime, int frequency, int gameTick);
    int getMapWidth() const override;
    int getMapHeight() const override;
    float getGameTime() const override;
    int getFrequency() const override;
    int getGameTick() const override;
    bool isMapInitialized() const override;
    bool isGameEnded() const override;
    const std::string& getWinningTeam() const override;
    std::shared_ptr<const ITile> getTile(int x, int y) const override;
    int getResourceQuantity(int x, int y, ResourceType resourceType) const override;
    ResourceType getDominantResourceType(int x, int y) const override;
    std::vector<int> getPlayersOnTile(int x, int y) const override;
    std::vector<int> getEggsOnTile(int x, int y) const override;
    std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const override;
    std::shared_ptr<const IPlayerInventory> getPlayerInventory(int playerId) const override;
    bool isPlayerOnTile(int x, int y, int playerId) const override;
    std::vector<int> getPlayerIds() const override;
    const std::vector<std::string>& getTeamNames() const override;
    std::vector<std::shared_ptr<const IBroadcast>> getBroadcasts() const override;
    std::vector<int> calculateTotalResources() const override;
    std::map<std::string, int> getTeamPlayerCounts() const override;
    std::map<std::string, std::vector<int>> getTeamResourceTotals() const override;

private:
    std::shared_ptr<const GameState> _gameState;
    mutable float _cachedGameTime = 0.0f;
    mutable int _cachedFrequency = 100;
    mutable int _cachedGameTick = 0;
    bool isValidGameState() const;
};

} // namespace GUI

#endif /* UI_DATA_PROVIDER_HPP_ */
