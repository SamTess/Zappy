/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MapSizeMessageHandler
*/

#ifndef MAP_SIZE_MESSAGE_HANDLER_HPP_
#define MAP_SIZE_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"

class MapSizeMessageHandler : public IMessageHandler {
public:
    explicit MapSizeMessageHandler(std::shared_ptr<GameState> gameState);
    ~MapSizeMessageHandler() = default;

    void handleMessage(std::shared_ptr<IMessageData> data) override;
    MessageType getMessageType() const override { return MessageType::MapSize; }

private:
    std::shared_ptr<GameState> _gameState;
};

#endif /* !MAP_SIZE_MESSAGE_HANDLER_HPP_ */
