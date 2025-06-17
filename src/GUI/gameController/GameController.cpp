/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameController implementation
*/

#include "GameController.hpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <string>

GameController::GameController(std::shared_ptr<NetworkManager> networkManager,
    std::shared_ptr<EntityFactoryManager> entityFactory) : _networkManager(networkManager) {
    std::cout << "  GAME CONTROLLER CREATED" << std::endl;
    _gameState = std::make_shared<GameState>(entityFactory);
    initializeMessageHandlers();
}

bool GameController::unknownPlayerId(int playerID) {
    if (_gameState->getPlayers().find(playerID)
        == _gameState->getPlayers().end()) {
        std::string ppo = "ppo #" + std::to_string(playerID) + "\n";
        std::string plv = "plv #" + std::to_string(playerID) + "\n";
        std::string pin = "pin #" + std::to_string(playerID) + "\n";
        std::cout << "PPO SENT!!!!!" << std::endl;
        _networkManager->sendCommand(ppo);
        std::cout << "PLV SENT!!!!!" << std::endl;
        _networkManager->sendCommand(plv);
        std::cout << "PIN SENT!!!!!" << std::endl;
        _networkManager->sendCommand(pin);
        return true;
    }
    return false;
}

// pas beau mais efficace
void GameController::initializeMessageHandlers() {
    _messageHandlers[MessageType::MapSize] = [this](std::shared_ptr<IMessageData> data) {
        handleMapSize(data);
    };
    _messageHandlers[MessageType::TileContent] = [this](std::shared_ptr<IMessageData> data) {
        handleTileContent(data);
    };
    _messageHandlers[MessageType::TeamName] = [this](std::shared_ptr<IMessageData> data) {
        handleTeamName(data);
    };
    _messageHandlers[MessageType::PlayerInfo] = [this](std::shared_ptr<IMessageData> data) {
        handlePlayerInfo(data);
    };
    _messageHandlers[MessageType::PlayerInventory] = [this](std::shared_ptr<IMessageData> data) {
        handlePlayerInventory(data);
    };
    _messageHandlers[MessageType::PlayerExpulsion] = [this](std::shared_ptr<IMessageData> data) {
        handlePlayerExpulsion(data);
    };
    _messageHandlers[MessageType::Broadcast] = [this](std::shared_ptr<IMessageData> data) {
        handlePlayerBroadcast(data);
    };
    _messageHandlers[MessageType::Incantation] = [this](std::shared_ptr<IMessageData> data) {
        handleIncantationStart(data);
    };
    _messageHandlers[MessageType::IncantationEnd] = [this](std::shared_ptr<IMessageData> data) {
        handleIncantationEnd(data);
    };
    _messageHandlers[MessageType::TimeUnit] = [this](std::shared_ptr<IMessageData> data) {
        handleTimeUnit(data);
    };
    _messageHandlers[MessageType::EndGame] = [this](std::shared_ptr<IMessageData> data) {
        handleEndGame(data);
    };
    _messageHandlers[MessageType::ServerMessage] = [this](std::shared_ptr<IMessageData> data) {
        handleServerMessage(data);
    };
    _messageHandlers[MessageType::Egg] = [this](std::shared_ptr<IMessageData> data) {
        handleEggDrop(data);
    };
}

void GameController::onMessageReceived(const Message& message) {
    processMessage(message);
}

// a faire autrement map surement
void GameController::processMessage(const Message& message) {
    if (!message.getStructuredData())
        return;
    MessageType messageType = message.getStructuredData()->getType();

    // la faire le check du map size
    if (messageType != MessageType::MapSize && !_gameState->isMapInitialized()) {
        auto mapSizeData = std::static_pointer_cast<MapSizeData>(message.getStructuredData());
        _gameState->setMapSize(mapSizeData->getWidth(), mapSizeData->getHeight());
    }
    auto it = _messageHandlers.find(messageType);
    if (it != _messageHandlers.end()) {
        it->second(message.getStructuredData());
    } else {
        std::cerr << "[GameController] No handler for message type: " << static_cast<int>(messageType) << std::endl;
    }
}

void GameController::handleMapSize(std::shared_ptr<IMessageData> data) {
    auto mapSizeData = std::static_pointer_cast<MapSizeData>(data);

    _gameState->setMapSize(mapSizeData->getWidth(), mapSizeData->getHeight());
}

void GameController::handleTileContent(std::shared_ptr<IMessageData> data) {
    if (!_gameState->isMapInitialized()) {
        std::cout << "MSZ SENT!!!!!" << std::endl;
        _networkManager->sendCommand("msz\n");
        std::cout << "MCT SENT!!!!!" << std::endl;
        _networkManager->sendCommand("mct\n");
        return;
    }
    auto tileData = std::static_pointer_cast<TileContentData>(data);
    int x = tileData->getX();
    int y = tileData->getY();

    _gameState->updateTileResources(x, y,
        tileData->getFood(), tileData->getLinemate(), tileData->getDeraumere(),
        tileData->getSibur(), tileData->getMendiane(), tileData->getPhiras(), tileData->getThystame());
}

void GameController::handleTeamName(std::shared_ptr<IMessageData> data) {
    auto teamData = std::static_pointer_cast<TeamNameData>(data);
    _gameState->setTeamNames(teamData->getNames());
}

void GameController::handlePlayerInfo(std::shared_ptr<IMessageData> data) {
    auto playerData = std::static_pointer_cast<PlayerInfoData>(data);
    if (playerData->getTeamName() != "" && unknownPlayerId(playerData->getId()))
        return;
    int playerId = playerData->getId();
    auto existingPlayer = _gameState->getPlayerInfo(playerId);

    if (existingPlayer && playerData->getOrientation() != -1) {
        if (!playerData->isAlive()) {
            _gameState->removePlayer(playerId);
            return;
        }
        int oldX = existingPlayer->getX();
        int oldY = existingPlayer->getY();
        int newX = playerData->getX();
        int newY = playerData->getY();
        if (oldX != newX || oldY != newY) {
            _gameState->addOrUpdatePlayer(*playerData);
            return;
        }
    }
    if (playerData->getOrientation() == -1) {
        playerData->setTeamName(existingPlayer->getTeamName());
        playerData->setX(existingPlayer->getX());
        playerData->setY(existingPlayer->getY());
        playerData->setOrientation(existingPlayer->getOrientation());
    }
    _gameState->addOrUpdatePlayer(*playerData);
}

void GameController::handlePlayerInventory(std::shared_ptr<IMessageData> data) {
    auto inventoryData = std::static_pointer_cast<PlayerInventoryData>(data);
    if (unknownPlayerId(inventoryData->getId()))
        return;

    _gameState->updatePlayerInventory(*inventoryData);
}

void GameController::handlePlayerExpulsion(std::shared_ptr<IMessageData> data) {
    auto expulsionData = std::static_pointer_cast<PlayerExpulsionData>(data);
    if (unknownPlayerId(expulsionData->getPlayerId()))
        return;
    // faire une animation ici
}

void GameController::handlePlayerBroadcast(std::shared_ptr<IMessageData> data) {
    auto broadcastData = std::static_pointer_cast<BroadcastData>(data);
    if (unknownPlayerId(broadcastData->getPlayerId()))
        return;
}

void GameController::handleResourceDrop(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);
    if (unknownPlayerId(resourceData->getPlayerId()))
        return;
}

void GameController::handleResourceCollect(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);
    if (unknownPlayerId(resourceData->getPlayerId()))
        return;
}

void GameController::handleIncantationStart(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationData>(data);
    int x = incantationData->getX();
    int y = incantationData->getY();
    // int level = incantationData->getLevel();
    for (int playerId : incantationData->getPlayerIds()) {
        unknownPlayerId(playerId);
    }

    _gameState->setTileIncantationState(x, y, true);
}

void GameController::handleIncantationEnd(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationEndData>(data);
    int x = incantationData->getX();
    int y = incantationData->getY();
    // bool success = incantationData->isSuccess();

    _gameState->setTileIncantationState(x, y, false);
}

void GameController::handleEggLaying(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    if (unknownPlayerId(eggData->getPlayerId()))
        return;
}

void GameController::handleEggDrop(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    if (unknownPlayerId(eggData->getPlayerId()))
        return;

    switch (eggData->getAction()) {
        case EggData::EggAction::Drop: {
            _gameState->addEgg(*eggData);
            break;
        }
        case EggData::EggAction::Connection:
            handleEggConnection(data);
            break;
        case EggData::EggAction::Death:
            handleEggDeath(data);
            break;
        case EggData::EggAction::Laying:
            handleEggLaying(data);
            break;
    }
}

void GameController::handleEggConnection(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    int eggId = eggData->getEggId();

    auto existingEgg = _gameState->getEggInfo(eggId);
    if (existingEgg)
        _gameState->removeEgg(eggId);
}

void GameController::handleEggDeath(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    int eggId = eggData->getEggId();
    auto existingEgg = _gameState->getEggInfo(eggId);

    if (existingEgg)
        _gameState->removeEgg(eggId);
}

void GameController::handleTimeUnit(std::shared_ptr<IMessageData> data) {
    auto timeData = std::static_pointer_cast<TimeUnitData>(data);
    _gameState->setTimeUnit(timeData->getTimeUnit());
}

void GameController::handleEndGame(std::shared_ptr<IMessageData> data) {
    auto endGameData = std::static_pointer_cast<EndGameData>(data);
    _gameState->setGameEnded(true, endGameData->getTeamName());
}

void GameController::handleServerMessage(std::shared_ptr<IMessageData> data) {
    auto serverData = std::static_pointer_cast<ServerMessageData>(data);
}

void GameController::setEntityFactory(std::shared_ptr<EntityFactoryManager> factory) {
    _gameState = std::make_shared<GameState>(factory);
    initializeMessageHandlers();
}
