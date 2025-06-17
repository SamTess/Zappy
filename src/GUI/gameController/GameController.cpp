/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameController implementation
*/

#include "GameController.hpp"
#include <iostream>
#include <algorithm>
#include <string>

GameController::GameController(std::shared_ptr<Logger> logger) : _gameState(std::make_shared<GameState>()), _logger(logger) {
    _logger->logInfo("GameController initialized with shared logger");
    initializeMessageHandlers();
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
    _logger->logInfo("Received message - Header: " + message.getHeader() +
        ", Raw data: " + message.getData().substr(0, message.getData().length()-1));
    processMessage(message);
}

// a faire autrement map surement
void GameController::processMessage(const Message& message) {
    if (!message.getStructuredData()) {
        _logger->logWarning("Message received without structured data: " + message.getHeader());
        return;
    }
    MessageType messageType = message.getStructuredData()->getType();

    auto it = _messageHandlers.find(messageType);
    if (it != _messageHandlers.end()) {
        _logger->logDebug("Processing message type: " + std::to_string(static_cast<int>(messageType)));
        it->second(message.getStructuredData());
    } else {
        _logger->logError("No handler for message type: " + std::to_string(static_cast<int>(messageType)));
        std::cerr << "[GameController] No handler for message type: " << static_cast<int>(messageType) << std::endl;
    }
}

void GameController::handleMapSize(std::shared_ptr<IMessageData> data) {
    auto mapSizeData = std::static_pointer_cast<MapSizeData>(data);

    _logger->logInfo("Setting map size: " + std::to_string(mapSizeData->getWidth()) +
        "x" + std::to_string(mapSizeData->getHeight()));
    _gameState->setMapSize(mapSizeData->getWidth(), mapSizeData->getHeight());
    _logger->logInfo("Map size set successfully");
}

void GameController::handleTileContent(std::shared_ptr<IMessageData> data) {
    auto tileData = std::static_pointer_cast<TileContentData>(data);
    int x = tileData->getX();
    int y = tileData->getY();
    _logger->logDebug("Updating tile content at (" + std::to_string(x) + "," + std::to_string(y) +
        ") - Food:" + std::to_string(tileData->getFood()) +
        " Linemate:" + std::to_string(tileData->getLinemate()) +
        " Deraumere:" + std::to_string(tileData->getDeraumere()) +
        " Sibur:" + std::to_string(tileData->getSibur()) +
        " Mendiane:" + std::to_string(tileData->getMendiane()) +
        " Phiras:" + std::to_string(tileData->getPhiras()) +
        " Thystame:" + std::to_string(tileData->getThystame()));

    _gameState->updateTileResources(x, y,
        tileData->getFood(), tileData->getLinemate(), tileData->getDeraumere(),
        tileData->getSibur(), tileData->getMendiane(), tileData->getPhiras(), tileData->getThystame());
}

void GameController::handleTeamName(std::shared_ptr<IMessageData> data) {
    auto teamData = std::static_pointer_cast<TeamNameData>(data);
    const auto& names = teamData->getNames();
    std::string teamList = "";
    for (size_t i = 0; i < names.size(); ++i) {
        teamList += names[i];
        if (i < names.size() - 1) teamList += ", ";
    }
    _logger->logInfo("Setting team names: [" + teamList + "]");
    _gameState->setTeamNames(teamData->getNames());
}

void GameController::handlePlayerInfo(std::shared_ptr<IMessageData> data) {
    auto playerData = std::static_pointer_cast<PlayerInfoData>(data);
    int playerId = playerData->getId();
    auto existingPlayer = _gameState->getPlayerInfo(playerId);

    if (existingPlayer) {
        if (!playerData->isAlive()) {
            _gameState->removePlayer(playerId);
            return;
        }
        int oldX = existingPlayer->getX();
        int oldY = existingPlayer->getY();
        int newX = playerData->getX();
        int newY = playerData->getY();
        if (oldX != newX || oldY != newY) {
            _logger->logInfo("Player " + std::to_string(playerId) + " moved from (" +
                std::to_string(oldX) + "," + std::to_string(oldY) + ") to (" +
                std::to_string(newX) + "," + std::to_string(newY) + ")");
            _gameState->addOrUpdatePlayer(*playerData);
            return;
        }
        _logger->logDebug("Updating player " + std::to_string(playerId) + " info (no movement)");
    } else {
        _logger->logInfo("New player " + std::to_string(playerId) + " added at (" +
            std::to_string(playerData->getX()) + "," + std::to_string(playerData->getY()) +
            ") - Team: " + playerData->getTeamName() +
            " Level: " + std::to_string(playerData->getLevel()) +
            " Orientation: " + std::to_string(playerData->getOrientation()));
    }
    _gameState->addOrUpdatePlayer(*playerData);
}

void GameController::handlePlayerInventory(std::shared_ptr<IMessageData> data) {
    auto inventoryData = std::static_pointer_cast<PlayerInventoryData>(data);
    int playerId = inventoryData->getId();

    _logger->logDebug("Updating inventory for player " + std::to_string(playerId) +
        " - Food:" + std::to_string(inventoryData->getFood()) +
        " Linemate:" + std::to_string(inventoryData->getLinemate()) +
        " Deraumere:" + std::to_string(inventoryData->getDeraumere()) +
        " Sibur:" + std::to_string(inventoryData->getSibur()) +
        " Mendiane:" + std::to_string(inventoryData->getMendiane()) +
        " Phiras:" + std::to_string(inventoryData->getPhiras()) +
        " Thystame:" + std::to_string(inventoryData->getThystame()));

    _gameState->updatePlayerInventory(*inventoryData);
}

void GameController::handlePlayerExpulsion(std::shared_ptr<IMessageData> data) {
    auto expulsionData = std::static_pointer_cast<PlayerExpulsionData>(data);
    _logger->logInfo("Player " + std::to_string(expulsionData->getPlayerId()) + " expelled");
    // faire une animation ici
}

void GameController::handlePlayerBroadcast(std::shared_ptr<IMessageData> data) {
    auto broadcastData = std::static_pointer_cast<BroadcastData>(data);
    _logger->logInfo("Player " + std::to_string(broadcastData->getPlayerId()) + " broadcast message: '" +
        broadcastData->getMessage() + "'");
}

void GameController::handleResourceDrop(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);

    _logger->logInfo("Player " + std::to_string(resourceData->getPlayerId()) + " dropped resource type " +
        std::to_string(resourceData->getResourceType()));
}

void GameController::handleResourceCollect(std::shared_ptr<IMessageData> data) {
    auto resourceData = std::static_pointer_cast<ResourceData>(data);

    _logger->logInfo("Player " + std::to_string(resourceData->getPlayerId()) + " collected resource type " +
        std::to_string(resourceData->getResourceType()));
}

void GameController::handlePlayerDeath(std::shared_ptr<IMessageData> data) {
    auto playerData = std::static_pointer_cast<PlayerInfoData>(data);
    int playerId = playerData->getId();

    _logger->logInfo("Player " + std::to_string(playerId) + " died and removed from game");
    _gameState->removePlayer(playerId);
}

void GameController::handleIncantationStart(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationData>(data);
    int x = incantationData->getX();
    int y = incantationData->getY();
    int level = incantationData->getLevel();

    _logger->logInfo("Incantation started at (" + std::to_string(x) + "," + std::to_string(y) +
        ") for level " + std::to_string(level));

    _gameState->setTileIncantationState(x, y, true);
}

void GameController::handleIncantationEnd(std::shared_ptr<IMessageData> data) {
    auto incantationData = std::static_pointer_cast<IncantationEndData>(data);
    int x = incantationData->getX();
    int y = incantationData->getY();
    bool success = incantationData->isSuccess();

    _logger->logInfo("Incantation ended at (" + std::to_string(x) + "," + std::to_string(y) +
        ") - Success: " + (success ? "true" : "false"));

    _gameState->setTileIncantationState(x, y, false);
}

void GameController::handleEggLaying(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);

    _logger->logInfo("Egg " + std::to_string(eggData->getEggId()) + " laid by player " +
        std::to_string(eggData->getPlayerId()) + " at (" +
        std::to_string(eggData->getX()) + "," + std::to_string(eggData->getY()) + ")");
}

void GameController::handleEggDrop(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);

    _logger->logDebug("Processing egg action for egg " + std::to_string(eggData->getEggId()));

    switch (eggData->getAction()) {
        case EggData::EggAction::Drop: {
            _logger->logInfo("Egg " + std::to_string(eggData->getEggId()) + " dropped at (" +
                std::to_string(eggData->getX()) + "," + std::to_string(eggData->getY()) + ")");
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
    if (existingEgg) {
        _logger->logInfo("Egg " + std::to_string(eggId) + " connected to player and removed from game");
        _gameState->removeEgg(eggId);
    } else {
        _logger->logWarning("Trying to connect non-existing egg " + std::to_string(eggId));
    }
}

void GameController::handleEggDeath(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);
    int eggId = eggData->getEggId();
    auto existingEgg = _gameState->getEggInfo(eggId);

    if (existingEgg) {
        _logger->logInfo("Egg " + std::to_string(eggId) + " died and removed from game");
        _gameState->removeEgg(eggId);
    } else {
        _logger->logWarning("Trying to remove non-existing egg " + std::to_string(eggId));
    }
}

void GameController::handleTimeUnit(std::shared_ptr<IMessageData> data) {
    auto timeData = std::static_pointer_cast<TimeUnitData>(data);
    int newTimeUnit = timeData->getTimeUnit();

    _logger->logDebug("Time unit updated to: " + std::to_string(newTimeUnit));
    _gameState->setTimeUnit(newTimeUnit);
}

void GameController::handleEndGame(std::shared_ptr<IMessageData> data) {
    auto endGameData = std::static_pointer_cast<EndGameData>(data);
    const std::string& winningTeam = endGameData->getTeamName();

    _logger->logInfo("Game ended! Winning team: " + winningTeam);
    _gameState->setGameEnded(true, winningTeam);
}

void GameController::handleServerMessage(std::shared_ptr<IMessageData> data) {
    auto serverData = std::static_pointer_cast<ServerMessageData>(data);

    _logger->logInfo("Server message: " + serverData->getMessage());
}
