/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EndGameMessageHandler
*/

#include "EndGameMessageHandler.hpp"

EndGameMessageHandler::EndGameMessageHandler(std::shared_ptr<GameState> gameState)
    : _gameState(gameState) {}

void EndGameMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto endGameData = std::static_pointer_cast<EndGameData>(data);
    _gameState->setGameEnded(true, endGameData->getTeamName());
}
