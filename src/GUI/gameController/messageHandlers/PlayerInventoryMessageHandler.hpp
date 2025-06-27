/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInventoryMessageHandler
*/

#ifndef PLAYER_INVENTORY_MESSAGE_HANDLER_HPP_
#define PLAYER_INVENTORY_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../playerValidationManager/PlayerValidationManager.hpp"

class PlayerInventoryMessageHandler : public IMessageHandler {
    public:
        PlayerInventoryMessageHandler(std::shared_ptr<GameState> gameState,
            std::shared_ptr<PlayerValidationManager> validator);
        ~PlayerInventoryMessageHandler() = default;

        void handleMessage(std::shared_ptr<IMessageData> data) override;
        MessageType getMessageType() const override { return MessageType::PlayerInventory; }

    private:
        std::shared_ptr<GameState> _gameState;
        std::shared_ptr<PlayerValidationManager> _validator;
        bool shouldValidatePlayer(std::shared_ptr<PlayerInventoryData> inventoryData);
        void updatePlayerInventory(std::shared_ptr<PlayerInventoryData> inventoryData);
};

#endif /* !PLAYER_INVENTORY_MESSAGE_HANDLER_HPP_ */
