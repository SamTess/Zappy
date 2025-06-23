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
#include "EntityFactory.hpp"
#include "../network/networkManager/NetworkManager.hpp"
#include "../Shared/IGraphicsLib.hpp"

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
class GameController {
public:
    GameController();
    GameController(std::shared_ptr<NetworkManager> networkManager, std::shared_ptr<EntityFactoryManager> entityFactory);
    ~GameController() = default;
    void onMessageReceived(const Message& message);
    std::shared_ptr<const GameState> getGameState() const { return _gameState; }
    void setEntityFactory(std::shared_ptr<EntityFactoryManager> factory);
    void setGraphics(std::shared_ptr<IGraphicsLib> graphics) { _graphics = graphics; }

    /**
     * @brief Met à jour les minuteurs de broadcasts
     * @param deltaTime Temps écoulé depuis la dernière mise à jour
     */
    void updateBroadcasts(float deltaTime);

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
    void handleIncantationStart(std::shared_ptr<IMessageData> data);
    void handleIncantationEnd(std::shared_ptr<IMessageData> data);
    void handleEggLaying(std::shared_ptr<IMessageData> data);
    void handleEggDrop(std::shared_ptr<IMessageData> data);
    void handleEggConnection(std::shared_ptr<IMessageData> data);
    void handleEggDeath(std::shared_ptr<IMessageData> data);
    void handleTimeUnit(std::shared_ptr<IMessageData> data);
    void handleEndGame(std::shared_ptr<IMessageData> data);
    void handleServerMessage(std::shared_ptr<IMessageData> data);
    bool unknownPlayerId(int playerID);

    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<NetworkManager> _networkManager;
    std::shared_ptr<IGraphicsLib> _graphics;
    std::map<MessageType, std::function<void(std::shared_ptr<IMessageData>)>> _messageHandlers;
    void initializeMessageHandlers();
};

#endif /* !GAME_CONTROLLER_HPP_ */
