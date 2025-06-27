/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggMessageHandler
*/

#include <memory>
#include "EggMessageHandler.hpp"

EggMessageHandler::EggMessageHandler(std::shared_ptr<GameState> gameState,
    std::shared_ptr<PlayerValidationManager> validator)
    : _gameState(gameState), _validator(validator) {}

void EggMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto eggData = std::static_pointer_cast<EggData>(data);

    switch (eggData->getAction()) {
        case EggAction::Drop:
            handleEggDrop(eggData);
            break;
        case EggAction::Connection:
            handleEggConnection(eggData);
            break;
        case EggAction::Death:
            handleEggDeath(eggData);
            break;
        case EggAction::Laying:
            handleEggLaying(eggData);
            break;
    }
}

void EggMessageHandler::handleEggDrop(std::shared_ptr<EggData> eggData) {
    _gameState->addEgg(*eggData);
}

void EggMessageHandler::handleEggConnection(std::shared_ptr<EggData> eggData) {
    int eggId = eggData->getEggId();
    auto existingEgg = _gameState->getEggInfo(eggId);
    if (existingEgg)
        _gameState->removeEgg(eggId);
}

void EggMessageHandler::handleEggDeath(std::shared_ptr<EggData> eggData) {
    int eggId = eggData->getEggId();
    auto existingEgg = _gameState->getEggInfo(eggId);
    if (existingEgg)
        _gameState->removeEgg(eggId);
}

void EggMessageHandler::handleEggLaying(std::shared_ptr<EggData> eggData) {
    if (_validator->validateAndRequestPlayerInfo(eggData->getPlayerId()))
        return;
}
