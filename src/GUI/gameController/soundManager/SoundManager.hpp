/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** SoundManager
*/

#ifndef SOUND_MANAGER_HPP_
#define SOUND_MANAGER_HPP_

#include <memory>
#include <string>
#include "../../../Shared/IGraphicsLib.hpp"
#include "../GameState.hpp"

class SoundManager {
    public:
        SoundManager(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<GameState> gameState);
        ~SoundManager() = default;

        void playDeathSound();
        void playBroadcastSound();
        void playSound(const std::string& soundPath);
        bool isSfxEnabled() const;

    private:
        std::shared_ptr<IGraphicsLib> _graphics;
        std::shared_ptr<GameState> _gameState;
};

#endif /* !SOUND_MANAGER_HPP_ */
