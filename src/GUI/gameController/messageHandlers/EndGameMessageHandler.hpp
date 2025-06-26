/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EndGameMessageHandler
*/

#ifndef END_GAME_MESSAGE_HANDLER_HPP_
#define END_GAME_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"

class EndGameMessageHandler : public IMessageHandler {
public:
    explicit EndGameMessageHandler(std::shared_ptr<GameState> gameState);
    ~EndGameMessageHandler() = default;

    void handleMessage(std::shared_ptr<IMessageData> data) override;
    MessageType getMessageType() const override { return MessageType::EndGame; }

private:
    std::shared_ptr<GameState> _gameState;
};

#endif /* !END_GAME_MESSAGE_HANDLER_HPP_ */
