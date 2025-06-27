/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IncantationEndMessageHandler
*/

#ifndef INCANTATION_END_MESSAGE_HANDLER_HPP_
#define INCANTATION_END_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"

class IncantationEndMessageHandler : public IMessageHandler {
    public:
        explicit IncantationEndMessageHandler(std::shared_ptr<GameState> gameState);
        ~IncantationEndMessageHandler() = default;

        void handleMessage(std::shared_ptr<IMessageData> data) override;
        MessageType getMessageType() const override { return MessageType::IncantationEnd; }

    private:
        std::shared_ptr<GameState> _gameState;
};

#endif /* !INCANTATION_END_MESSAGE_HANDLER_HPP_ */
