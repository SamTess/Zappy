/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TeamNameMessageHandler
*/

#include <memory>
#include "TeamNameMessageHandler.hpp"

TeamNameMessageHandler::TeamNameMessageHandler(std::shared_ptr<GameState> gameState)
    : _gameState(gameState) {}

void TeamNameMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto teamData = std::static_pointer_cast<TeamNameData>(data);
    _gameState->setTeamNames(teamData->getNames());
}
