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
#include <chrono>

#include "GameState.hpp"
#include "EntityFactory.hpp"
#include "../shared/commands/ICommand.hpp"
#include "../Shared/IGraphicsLib.hpp"
#include "../renderer/EjectionAnimationManager.hpp"
#include "../renderer/DeathAnimationManager.hpp"
#include "../shared/Message.hpp"
#include "../shared/commands/ICommandExecutor.hpp"
#include "GameNetworkCommand.hpp"

class GameController {
public:
    GameController();
    GameController(std::shared_ptr<ICommandExecutor> commandExecutor, std::shared_ptr<EntityFactoryManager> entityFactory);
    ~GameController() = default;
    std::shared_ptr<const GameState> getGameState() const { return _gameState; }
    void setEntityFactory(std::shared_ptr<EntityFactoryManager> factory);
    void setGraphics(std::shared_ptr<IGraphicsLib> graphics) { _graphics = graphics; }
    void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor);
    void updateAnimations(float deltaTime);
    void processMessage(const Message& message);

private:
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
    void initializeMessageHandlers();

    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<ICommandExecutor> _commandExecutor;
    std::shared_ptr<IGraphicsLib> _graphics;
    std::map<MessageType, std::function<void(std::shared_ptr<IMessageData>)>> _messageHandlers;
    std::map<int, std::chrono::steady_clock::time_point> _lastBroadcastTime;
};

#endif /* !GAME_CONTROLLER_HPP_ */
