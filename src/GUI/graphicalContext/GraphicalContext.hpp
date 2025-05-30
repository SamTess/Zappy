/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicalContext
*/

#ifndef GRAPHICAL_CONTEXT_HPP_
#define GRAPHICAL_CONTEXT_HPP_

#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <string>

#include "../network/protocol/Message.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"
#include "../network/protocol/HeaderMessage.hpp"


class GraphicalContext {
public:
    typedef void (GraphicalContext::*UpdateFunction)(std::shared_ptr<IMessageData>);
    GraphicalContext();
    ~GraphicalContext() = default;

    void updateContext(const Message& message);

private:
    void updateMapSize(std::shared_ptr<IMessageData> data);
    void updateTileContent(std::shared_ptr<IMessageData> data);
    void updateTeamName(std::shared_ptr<IMessageData> data);
    void updatePlayerInfo(std::shared_ptr<IMessageData> data);
    void updatePlayerInventory(std::shared_ptr<IMessageData> data);
    void updatePlayerExpulsion(std::shared_ptr<IMessageData> data);
    void updatePlayerBroadcast(std::shared_ptr<IMessageData> data);
    void updateResourceDrop(std::shared_ptr<IMessageData> data);
    void updateResourceCollect(std::shared_ptr<IMessageData> data);
    void updatePlayerDeath(std::shared_ptr<IMessageData> data);
    void updateIncantationStart(std::shared_ptr<IMessageData> data);
    void updateIncantationEnd(std::shared_ptr<IMessageData> data);
    void updateEggLaying(std::shared_ptr<IMessageData> data);
    void updateEggDrop(std::shared_ptr<IMessageData> data);
    void updateEggConnection(std::shared_ptr<IMessageData> data);
    void updateEggDeath(std::shared_ptr<IMessageData> data);
    void updateTimeUnit(std::shared_ptr<IMessageData> data);
    void updateEndGame(std::shared_ptr<IMessageData> data);
    void updateServerMessage(std::shared_ptr<IMessageData> data);


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
