/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TimeUnitMessageHandler
*/

#ifndef TIME_UNIT_MESSAGE_HANDLER_HPP_
#define TIME_UNIT_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"

class TimeUnitMessageHandler : public IMessageHandler {
    public:
        explicit TimeUnitMessageHandler(std::shared_ptr<GameState> gameState);
        ~TimeUnitMessageHandler() = default;

        void handleMessage(std::shared_ptr<IMessageData> data) override;
        MessageType getMessageType() const override { return MessageType::TimeUnit; }

    private:
        std::shared_ptr<GameState> _gameState;
};

#endif /* !TIME_UNIT_MESSAGE_HANDLER_HPP_ */
