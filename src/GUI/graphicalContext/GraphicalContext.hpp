/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicalContext
*/

#ifndef GRAPHICAL_CONTEXT_HPP_
#define GRAPHICAL_CONTEXT_HPP_

#include "../network/protocol/Message.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"
#include "../network/protocol/HeaderMessage.hpp"

#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>

class GraphicalContext {
public:
    typedef std::function<void(std::shared_ptr<IMessageData>)> UpdateFunction;
    GraphicalContext();
    ~GraphicalContext() = default;

    void updateContext(const Message& message);

private:

    void updateMapSize(std::shared_ptr<MapSizeData> data);
    void updateTileContent(std::shared_ptr<TileContentData> data);
    void updateTeamName(std::shared_ptr<TeamNameData> data);
    void updatePlayerInfo(std::shared_ptr<PlayerInfoData> data);
    void updatePlayerInventory(std::shared_ptr<PlayerInventoryData> data);
    void updatePlayerExpulsion(std::shared_ptr<PlayerInfoData> data);
    void updatePlayerBroadcast(std::shared_ptr<BroadcastData> data);
    void updateResourceDrop(std::shared_ptr<ResourceData> data);
    void updateResourceCollect(std::shared_ptr<ResourceData> data);
    void updatePlayerDeath(std::shared_ptr<PlayerInfoData> data);
    void updateIncantationStart(std::shared_ptr<IncantationData> data);
    void updateIncantationEnd(std::shared_ptr<IncantationEndData> data);
    void updateEggLaying(std::shared_ptr<EggData> data);
    void updateEggDrop(std::shared_ptr<EggData> data);
    void updateEggConnection(std::shared_ptr<EggData> data);
    void updateEggDeath(std::shared_ptr<EggData> data);
    void updateTimeUnit(std::shared_ptr<TimeUnitData> data);
    void updateEndGame(std::shared_ptr<EndGameData> data);
    void updateServerMessage(std::shared_ptr<ServerMessageData> data);


    std::vector<std::string> _teamNames;
    std::map<int, PlayerInfoData> _players;
    std::map<int, PlayerInventoryData> _inventories;
    std::map<int, EggData> _eggs;
    int _timeUnit = 100;
    bool _gameEnded = false;
    std::string _winningTeam;

    std::map<MessageType, UpdateFunction> _updateFunctions;
};

#endif /* !GRAPHICAL_CONTEXT_HPP_ */
