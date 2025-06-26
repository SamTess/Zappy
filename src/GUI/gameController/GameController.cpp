/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameController
*/

#include <memory>
#include "GameController.hpp"

GameController::GameController() {
    _gameState = std::make_shared<GameState>(std::make_shared<EntityFactoryManager>());
    initializeManagers();
    registerMessageHandlers();
}

GameController::GameController(
    std::shared_ptr<ICommandExecutor> commandExecutor,
    std::shared_ptr<EntityFactoryManager> entityFactory) {
    _gameState = std::make_shared<GameState>(entityFactory);
    initializeManagers();
    if (commandExecutor)
        _networkManager->setCommandExecutor(commandExecutor);
    registerMessageHandlers();
}

void GameController::initializeManagers() {
    _graphicsManager = std::make_shared<GraphicsManager>(nullptr);
    _animationManager = std::make_shared<AnimationManager>(_gameState);
    _soundManager = std::make_shared<SoundManager>(_graphicsManager->getGraphics(), _gameState);
    _networkManager = std::make_shared<NetworkCommandManager>(nullptr);
    _playerValidator = std::make_shared<PlayerValidationManager>(_gameState, _networkManager);
    _messageRegistry = std::make_shared<MessageHandlerRegistry>();
}
void GameController::registerMessageHandlers() {
    _messageRegistry->registerHandler(std::make_shared<MapSizeMessageHandler>(_gameState));
    _messageRegistry->registerHandler(std::make_shared<TileContentMessageHandler>(_gameState, _networkManager));
    _messageRegistry->registerHandler(std::make_shared<TimeUnitMessageHandler>(_gameState));
    _messageRegistry->registerHandler(std::make_shared<PlayerInfoMessageHandler>(
        _gameState, _playerValidator, _animationManager, _soundManager));
    _messageRegistry->registerHandler(std::make_shared<PlayerExpulsionMessageHandler>(
        _gameState, _playerValidator, _animationManager));
    _messageRegistry->registerHandler(std::make_shared<PlayerInventoryMessageHandler>(
        _gameState, _playerValidator));
    _messageRegistry->registerHandler(std::make_shared<TeamNameMessageHandler>(_gameState));
    _messageRegistry->registerHandler(std::make_shared<BroadcastMessageHandler>(
        _gameState, _soundManager, _animationManager));
    _messageRegistry->registerHandler(std::make_shared<IncantationMessageHandler>(
        _gameState, _playerValidator));
    _messageRegistry->registerHandler(std::make_shared<IncantationEndMessageHandler>(_gameState));
    _messageRegistry->registerHandler(std::make_shared<EggMessageHandler>(_gameState, _playerValidator));
    _messageRegistry->registerHandler(std::make_shared<EndGameMessageHandler>(_gameState));
}

void GameController::setEntityFactory(std::shared_ptr<EntityFactoryManager> factory) {
    if (_gameState)
        return;
    _gameState = std::make_shared<GameState>(factory);
    initializeManagers();
    registerMessageHandlers();
}

void GameController::setGraphics(std::shared_ptr<IGraphicsLib> graphics) {
    _graphicsManager->setGraphics(graphics);
    _soundManager = std::make_shared<SoundManager>(graphics, _gameState);
}

void GameController::setCommandExecutor(std::shared_ptr<ICommandExecutor> executor) {
    _networkManager->setCommandExecutor(executor);
}

void GameController::updateAnimations(float deltaTime) {
    _animationManager->updateAnimations(deltaTime);
}

void GameController::processMessage(const Message& message) {
    _messageRegistry->processMessage(message);
}
