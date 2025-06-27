/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TimeUnitMessageHandler
*/

#include <memory>
#include "TimeUnitMessageHandler.hpp"

TimeUnitMessageHandler::TimeUnitMessageHandler(std::shared_ptr<GameState> gameState)
    : _gameState(gameState) {}

void TimeUnitMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto timeData = std::static_pointer_cast<TimeUnitData>(data);
    _gameState->setTimeUnit(timeData->getTimeUnit());
}
