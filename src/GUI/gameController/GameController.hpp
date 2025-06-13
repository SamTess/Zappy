/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameController
*/

#ifndef GAME_CONTROLLER_HPP_
#define GAME_CONTROLLER_HPP_

#include <memory>
#include <vector>
#include <functional>
#include <map>

#include "../network/protocol/Message.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"
#include "../network/protocol/HeaderMessage.hpp"
#include "../network/protocol/ProtocolParser.hpp"
#include "GameState.hpp"

/**
 * @brief Interface pour recevoir les messages du réseau
 */
class INetworkObserver {
public:
    virtual ~INetworkObserver() = default;
    virtual void onMessageReceived(const Message& message) = 0;
};

/**
 * @brief Contrôleur principal du jeu - gère la logique métier et orchestre les mises à jour
 */
class GameController : public INetworkObserver {
public:
    GameController();
    ~GameController() = default;

    void onMessageReceived(const Message& message) override;

    std::shared_ptr<const GameState> getGameState() const { return _gameState; }

private:
    void processMessage(const Message& message);
    void handleMapSize(std::shared_ptr<IMessageData> data);
    void handleTileContent(std::shared_ptr<IMessageData> data);
    void handleTeamName(std::shared_ptr<IMessageData> data);
    void handlePlayerInfo(std::shared_ptr<IMessageData> data);
    void handlePlayerInventory(std::shared_ptr<IMessageData> data);
    void handlePlayerExpulsion(std::shared_ptr<IMessageData> data);
    void handlePlayerBroadcast(std::shared_ptr<IMessageData> data);
    void handleResourceDrop(std::shared_ptr<IMessageData> data);
    void handleResourceCollect(std::shared_ptr<IMessageData> data);
    void handlePlayerDeath(std::shared_ptr<IMessageData> data);
    void handleIncantationStart(std::shared_ptr<IMessageData> data);
    void handleIncantationEnd(std::shared_ptr<IMessageData> data);
    void handleEggLaying(std::shared_ptr<IMessageData> data);
    void handleEggDrop(std::shared_ptr<IMessageData> data);
    void handleEggConnection(std::shared_ptr<IMessageData> data);
    void handleEggDeath(std::shared_ptr<IMessageData> data);
    void handleTimeUnit(std::shared_ptr<IMessageData> data);
    void handleEndGame(std::shared_ptr<IMessageData> data);
    void handleServerMessage(std::shared_ptr<IMessageData> data);

    std::shared_ptr<GameState> _gameState;
};

#endif /* !GAME_CONTROLLER_HPP_ */
