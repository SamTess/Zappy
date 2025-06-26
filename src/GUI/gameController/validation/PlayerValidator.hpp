/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerValidator - Validation et gestion des joueurs
*/

#ifndef PLAYER_VALIDATOR_HPP_
#define PLAYER_VALIDATOR_HPP_

#include <memory>
#include "../GameState.hpp"
#include "../networkCommandManager/NetworkCommandManager.hpp"

class PlayerValidator {
public:
    PlayerValidator(std::shared_ptr<GameState> gameState,
                   std::shared_ptr<NetworkCommandManager> networkManager);
    ~PlayerValidator() = default;

    bool isPlayerKnown(int playerId);
    bool validateAndRequestPlayerData(int playerId);

private:
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<NetworkCommandManager> _networkManager;
};

#endif /* !PLAYER_VALIDATOR_HPP_ */
