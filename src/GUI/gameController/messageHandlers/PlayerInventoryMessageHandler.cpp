/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInventoryMessageHandler
*/

#include <memory>
#include "PlayerInventoryMessageHandler.hpp"

PlayerInventoryMessageHandler::PlayerInventoryMessageHandler(
    std::shared_ptr<GameState> gameState,
    std::shared_ptr<PlayerValidationManager> validator)
    : _gameState(gameState), _validator(validator) {}

void PlayerInventoryMessageHandler::handleMessage(std::shared_ptr<IMessageData> data) {
    auto inventoryData = std::static_pointer_cast<PlayerInventoryData>(data);

    if (shouldValidatePlayer(inventoryData))
        return;
        
    updatePlayerInventory(inventoryData);
}

bool PlayerInventoryMessageHandler::shouldValidatePlayer(std::shared_ptr<PlayerInventoryData> inventoryData) {
    return _validator->validateAndRequestPlayerInfo(inventoryData->getId());
}

void PlayerInventoryMessageHandler::updatePlayerInventory(std::shared_ptr<PlayerInventoryData> inventoryData) {
    _gameState->updatePlayerInventory(*inventoryData);
}
}
