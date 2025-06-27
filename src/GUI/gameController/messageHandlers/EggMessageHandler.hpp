/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggMessageHandler
*/

#ifndef EGG_MESSAGE_HANDLER_HPP_
#define EGG_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../playerValidationManager/PlayerValidationManager.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"

class EggMessageHandler : public IMessageHandler {
    public:
        EggMessageHandler(std::shared_ptr<GameState> gameState,
                        std::shared_ptr<PlayerValidationManager> validator);
        ~EggMessageHandler() = default;

        void handleMessage(std::shared_ptr<IMessageData> data) override;
        MessageType getMessageType() const override { return MessageType::Egg; }

    private:
        std::shared_ptr<GameState> _gameState;
        std::shared_ptr<PlayerValidationManager> _validator;

        void handleEggDrop(std::shared_ptr<EggData> eggData);
        void handleEggConnection(std::shared_ptr<EggData> eggData);
        void handleEggDeath(std::shared_ptr<EggData> eggData);
        void handleEggLaying(std::shared_ptr<EggData> eggData);
};

#endif /* !EGG_MESSAGE_HANDLER_HPP_ */
