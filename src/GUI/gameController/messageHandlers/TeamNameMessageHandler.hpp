/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TeamNameMessageHandler
*/

#ifndef TEAM_NAME_MESSAGE_HANDLER_HPP_
#define TEAM_NAME_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"

class TeamNameMessageHandler : public IMessageHandler {
public:
    explicit TeamNameMessageHandler(std::shared_ptr<GameState> gameState);
    ~TeamNameMessageHandler() = default;

    void handleMessage(std::shared_ptr<IMessageData> data) override;
    MessageType getMessageType() const override { return MessageType::TeamName; }

private:
    std::shared_ptr<GameState> _gameState;
};

#endif /* !TEAM_NAME_MESSAGE_HANDLER_HPP_ */
