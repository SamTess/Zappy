/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerExpulsionMessageHandler
*/

#ifndef PLAYER_EXPULSION_MESSAGE_HANDLER_HPP_
#define PLAYER_EXPULSION_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../playerValidationManager/PlayerValidationManager.hpp"
#include "../animationManager/AnimationManager.hpp"
#include "../../renderer/EjectionAnimationManager.hpp"

class PlayerExpulsionMessageHandler : public IMessageHandler {
    public:
        PlayerExpulsionMessageHandler(std::shared_ptr<GameState> gameState,
                                    std::shared_ptr<PlayerValidationManager> validator,
                                    std::shared_ptr<AnimationManager> animationManager);
        ~PlayerExpulsionMessageHandler() = default;

        void handleMessage(std::shared_ptr<IMessageData> data) override;
        MessageType getMessageType() const override { return MessageType::PlayerExpulsion; }

    private:
        std::shared_ptr<GameState> _gameState;
        std::shared_ptr<PlayerValidationManager> _validator;
        std::shared_ptr<AnimationManager> _animationManager;

        ZappyTypes::Vector3 calculateEjectionDirection(int orientation);
};

#endif /* !PLAYER_EXPULSION_MESSAGE_HANDLER_HPP_ */
