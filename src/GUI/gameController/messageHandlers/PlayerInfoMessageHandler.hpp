/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInfoMessageHandler
*/

#ifndef PLAYER_INFO_MESSAGE_HANDLER_HPP_
#define PLAYER_INFO_MESSAGE_HANDLER_HPP_

#include <memory>
#include "IMessageHandler.hpp"
#include "../GameState.hpp"
#include "../playerValidationManager/PlayerValidationManager.hpp"
#include "../animationManager/AnimationManager.hpp"
#include "../soundManager/SoundManager.hpp"

class PlayerInfoMessageHandler : public IMessageHandler {
public:
    PlayerInfoMessageHandler(std::shared_ptr<GameState> gameState,
        std::shared_ptr<PlayerValidationManager> validator,
        std::shared_ptr<AnimationManager> animationManager,
        std::shared_ptr<SoundManager> soundManager);
    ~PlayerInfoMessageHandler() = default;

    void handleMessage(std::shared_ptr<IMessageData> data) override;
    MessageType getMessageType() const override { return MessageType::PlayerInfo; }

private:
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<PlayerValidationManager> _validator;
    std::shared_ptr<AnimationManager> _animationManager;
    std::shared_ptr<SoundManager> _soundManager;

    bool handlePlayerValidation(std::shared_ptr<PlayerInfoData> playerData,
        std::shared_ptr<const IPlayer> existingPlayer);
    bool handleExistingPlayerWithValidOrientation(std::shared_ptr<PlayerInfoData> playerData,
        std::shared_ptr<const IPlayer> existingPlayer, int playerId);
    void updatePlayerInGameState(std::shared_ptr<PlayerInfoData> playerData);
    void handleMissingTeamInfo(std::shared_ptr<PlayerInfoData> playerData,
        std::shared_ptr<const IPlayer> existingPlayer);
    bool handlePlayerDeath(std::shared_ptr<PlayerInfoData> playerData,
        std::shared_ptr<const IPlayer> existingPlayer, int playerId);
    void processPlayerDeath(std::shared_ptr<const IPlayer> existingPlayer, int playerId);
    bool handlePlayerMovement(std::shared_ptr<PlayerInfoData> playerData,
        std::shared_ptr<const IPlayer> existingPlayer);
    void handleIncompleteOrientationData(std::shared_ptr<PlayerInfoData> playerData,
        std::shared_ptr<const IPlayer> existingPlayer);
};

#endif /* !PLAYER_INFO_MESSAGE_HANDLER_HPP_ */
