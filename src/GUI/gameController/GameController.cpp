/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameController implementation
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include "GameController.hpp"
#include "../renderer/EjectionAnimationManager.hpp"
#include "../renderer/ParticleSystem.hpp"


GameController::GameController(std::shared_ptr<NetworkManager> networkManager,
    std::shared_ptr<EntityFactoryManager> entityFactory) : _networkManager(networkManager) {
    _gameState = std::make_shared<GameState>(entityFactory);
    initializeMessageHandlers();
}

bool GameController::unknownPlayerId(int playerID) {
    const auto &players = _gameState->getPlayers();
    if (players.empty() || (!players.empty() && players.find(playerID) == players.end())) {
        std::string ppo = "ppo #" + std::to_string(playerID) + "\n";
        std::string plv = "plv #" + std::to_string(playerID) + "\n";
        std::string pin = "pin #" + std::to_string(playerID) + "\n";
        _networkManager->sendCommand(ppo);
        _networkManager->sendCommand(plv);
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
        _networkManager->sendCommand("msz\n");
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
    int playerId = playerData->getId();
    auto existingPlayer = _gameState->getPlayerInfo(playerId);

    if (!existingPlayer && playerData->getTeamName().empty()) {
        if (unknownPlayerId(playerData->getId()))
            return;
    }

    if (existingPlayer && playerData->getTeamName().empty()) {
        playerData->setTeamName(existingPlayer->getTeamName());
    }
    if (existingPlayer && playerData->getOrientation() != -1) {
        if (!playerData->isAlive()) {
            ZappyTypes::Vector3 playerWorldPos = Zappy::EjectionAnimationManager::getInstance().convertTileToWorldPosition(
                existingPlayer->getX(), existingPlayer->getY(), _gameState->getMapWidth(), _gameState->getMapHeight());
            Zappy::DeathAnimationManager::getInstance().startDeathAnimation(playerId, playerWorldPos, existingPlayer->getTeamName());
            _gameState->removePlayer(playerId);
            _graphics->PlaySound("assets/music/death.mp3");
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
        if (existingPlayer) {
            playerData->setTeamName(existingPlayer->getTeamName());
            playerData->setX(existingPlayer->getX());
            playerData->setY(existingPlayer->getY());
            playerData->setOrientation(existingPlayer->getOrientation());
        }
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
    int playerId = expulsionData->getPlayerId();

    if (unknownPlayerId(playerId))
        return;

    auto playerInfo = _gameState->getPlayerInfo(playerId);
    if (!playerInfo)
        return;
    int mapWidth = _gameState->getMapWidth();
    int mapHeight = _gameState->getMapHeight();

    ZappyTypes::Vector3 currentPos = Zappy::EjectionAnimationManager::getInstance().convertTileToWorldPosition(
        playerInfo->getX(), playerInfo->getY(), mapWidth, mapHeight);

    int orientation = playerInfo->getOrientation();
    ZappyTypes::Vector3 ejectionDirection = {0.0f, 0.0f, 0.0f};

    switch (orientation) {
        case 1:
            ejectionDirection = {0.0f, 0.0f, -1.0f};
            break;
        case 2:
            ejectionDirection = {1.0f, 0.0f, 0.0f};
            break;
        case 3:
            ejectionDirection = {0.0f, 0.0f, 1.0f};
            break;
        case 4:
            ejectionDirection = {-1.0f, 0.0f, 0.0f};
            break;
        default:
            ejectionDirection = {1.0f, 0.0f, 0.0f};
            break;
    }

    int destTileX = playerInfo->getX() + static_cast<int>(ejectionDirection.x);
    int destTileY = playerInfo->getY() + static_cast<int>(ejectionDirection.z);

    ZappyTypes::Vector3 destinationPos = Zappy::EjectionAnimationManager::getInstance().convertTileToWorldPosition(
        destTileX, destTileY, mapWidth, mapHeight);

    Zappy::EjectionAnimationManager::getInstance().startEjectionAnimation(
        playerId, currentPos, destinationPos);
}

void GameController::handlePlayerBroadcast(std::shared_ptr<IMessageData> data) {
    auto broadcastData = std::static_pointer_cast<BroadcastData>(data);
    int playerId = broadcastData->getPlayerId();
    const std::string& message = broadcastData->getMessage();

    if (message.empty() || playerId < 0) {
        std::cout << "[DEBUG] Skipping invalid broadcast - Player ID: " << playerId << ", Message: '" << message << "'" << std::endl;
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto it = _lastBroadcastTime.find(playerId);
    if (it != _lastBroadcastTime.end()) {
        auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second);
        if (timeDiff.count() < 500) {
            std::cout << "[DEBUG] Broadcast throttled for player " << playerId << " (too frequent)" << std::endl;
            return;
        }
    }
    _lastBroadcastTime[playerId] = now;

    std::cout << "[DEBUG] Broadcast triggered - Player ID: " << playerId << ", Message: '" << message << "'" << std::endl;

    std::string teamName = "Inconnu";
    auto playerInfo = _gameState->getPlayerInfo(playerId);
    if (playerInfo) {
        teamName = playerInfo->getTeamName();
        ZappyTypes::Vector3 playerWorldPos = Zappy::EjectionAnimationManager::getInstance().convertTileToWorldPosition(
            playerInfo->getX(), playerInfo->getY(), _gameState->getMapWidth(), _gameState->getMapHeight());

        Zappy::ParticleSystem::getInstance().createPlayerBroadcastEffect(playerWorldPos, teamName);
    } else {
        std::cout << "[DEBUG] Player " << playerId << " not found in game state - broadcast animation skipped" << std::endl;
    }
    _gameState->addBroadcast(playerId, teamName, message);
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
    // server au debut c'est -1 donc il affiche pas les oeuf de depart
    // if (unknownPlayerId(eggData->getPlayerId()))
    //     return;

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

void GameController::updateBroadcasts(float deltaTime) {
    _gameState->updateBroadcasts(deltaTime);
}

void GameController::updateAnimations(float deltaTime) {
    Zappy::EjectionAnimationManager::getInstance().update(deltaTime);
    Zappy::ParticleSystem::getInstance().update(deltaTime);
    Zappy::DeathAnimationManager::getInstance().update(deltaTime);
}

void GameController::setEntityFactory(std::shared_ptr<EntityFactoryManager> factory) {
    _gameState = std::make_shared<GameState>(factory);
    initializeMessageHandlers();
}
