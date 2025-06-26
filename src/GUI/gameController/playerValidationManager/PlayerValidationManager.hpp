/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerValidationManager
*/

#ifndef PLAYER_VALIDATION_MANAGER_HPP_
#define PLAYER_VALIDATION_MANAGER_HPP_

#include <memory>
#include "../GameState.hpp"

class NetworkCommandManager;

class PlayerValidationManager {
public:
    PlayerValidationManager(std::shared_ptr<GameState> gameState,
                           std::shared_ptr<NetworkCommandManager> networkManager);
    ~PlayerValidationManager() = default;

    bool isPlayerKnown(int playerId) const;
    bool validateAndRequestPlayerInfo(int playerId);

private:
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<NetworkCommandManager> _networkManager;
};

#endif /* !PLAYER_VALIDATION_MANAGER_HPP_ */
