/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BroadcastMessageHandler
*/

#ifndef BROADCAST_MESSAGE_HANDLER_HPP_
#define BROADCAST_MESSAGE_HANDLER_HPP_

#include <chrono>
#include <map>
#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../soundManager/SoundManager.hpp"
#include "../animationManager/AnimationManager.hpp"

class BroadcastMessageHandler : public IMessageHandler {
    public:
        BroadcastMessageHandler(std::shared_ptr<GameState> gameState,
            std::shared_ptr<SoundManager> soundManager,
            std::shared_ptr<AnimationManager> animationManager);
        ~BroadcastMessageHandler() = default;
        void handleMessage(std::shared_ptr<IMessageData> data) override;
        MessageType getMessageType() const override { return MessageType::Broadcast; }

    private:
        std::shared_ptr<GameState> _gameState;
        std::shared_ptr<SoundManager> _soundManager;
        std::shared_ptr<AnimationManager> _animationManager;
        std::map<int, std::chrono::steady_clock::time_point> _lastBroadcastTime;
        bool shouldThrottleBroadcast(int playerId);
};

#endif /* !BROADCAST_MESSAGE_HANDLER_HPP_ */
