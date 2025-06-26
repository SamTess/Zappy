/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameController
*/

#ifndef REFACTORED_GAME_CONTROLLER_HPP_
#define REFACTORED_GAME_CONTROLLER_HPP_

#include <memory>
#include "GameState.hpp"
#include "EntityFactory.hpp"
#include "IGameController.hpp"
#include "../shared/commands/ICommand.hpp"
#include "../Shared/IGraphicsLib.hpp"
#include "../shared/Message.hpp"
#include "../shared/commands/ICommandExecutor.hpp"

// Managers
#include "messageHandlers/MessageHandlerRegistry.hpp"
#include "animationManager/AnimationManager.hpp"
#include "soundManager/SoundManager.hpp"
#include "networkCommandManager/NetworkCommandManager.hpp"
#include "playerValidationManager/PlayerValidationManager.hpp"
#include "graphicsManager/GraphicsManager.hpp"

// Message Handlers
#include "messageHandlers/MapSizeMessageHandler.hpp"
#include "messageHandlers/TileContentMessageHandler.hpp"
#include "messageHandlers/PlayerInfoMessageHandler.hpp"
#include "messageHandlers/PlayerExpulsionMessageHandler.hpp"
#include "messageHandlers/BroadcastMessageHandler.hpp"
#include "messageHandlers/TeamNameMessageHandler.hpp"
#include "messageHandlers/PlayerInventoryMessageHandler.hpp"
#include "messageHandlers/EggMessageHandler.hpp"
#include "messageHandlers/IncantationEndMessageHandler.hpp"
#include "messageHandlers/IncantationMessageHandler.hpp"
#include "messageHandlers/EndGameMessageHandler.hpp"
#include "messageHandlers/TimeUnitMessageHandler.hpp"

class GameController : public IGameController {
public:
    GameController();
    GameController(std::shared_ptr<ICommandExecutor> commandExecutor,
                           std::shared_ptr<EntityFactoryManager> entityFactory);
    ~GameController() = default;
    std::shared_ptr<const GameState> getGameState() const override { return _gameState; }
    void setEntityFactory(std::shared_ptr<EntityFactoryManager> factory) override;
    void setGraphics(std::shared_ptr<IGraphicsLib> graphics) override;
    void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor) override;
    void updateAnimations(float deltaTime) override;
    void processMessage(const Message& message) override;

private:
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<MessageHandlerRegistry> _messageRegistry;
    std::shared_ptr<AnimationManager> _animationManager;
    std::shared_ptr<SoundManager> _soundManager;
    std::shared_ptr<NetworkCommandManager> _networkManager;
    std::shared_ptr<PlayerValidationManager> _playerValidator;
    std::shared_ptr<GraphicsManager> _graphicsManager;

    void initializeManagers();
    void registerMessageHandlers();
};

#endif /* !REFACTORED_GAME_CONTROLLER_HPP_ */
