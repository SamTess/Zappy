/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IncantationMessageHandler
*/

#ifndef INCANTATION_MESSAGE_HANDLER_HPP_
#define INCANTATION_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../playerValidationManager/PlayerValidationManager.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"

class IncantationMessageHandler : public IMessageHandler {
public:
    IncantationMessageHandler(std::shared_ptr<GameState> gameState,
                            std::shared_ptr<PlayerValidationManager> validator);
    ~IncantationMessageHandler() = default;

    void handleMessage(std::shared_ptr<IMessageData> data) override;
    MessageType getMessageType() const override { return MessageType::Incantation; }

private:
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<PlayerValidationManager> _validator;
};

#endif /* !INCANTATION_MESSAGE_HANDLER_HPP_ */
