/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** SoundManager
*/

#include <memory>
#include <string>
#include <iostream>
#include "SoundManager.hpp"

SoundManager::SoundManager(std::shared_ptr<IGraphicsLib> graphics, std::shared_ptr<GameState> gameState)
    : _graphics(graphics), _gameState(gameState) {}

void SoundManager::playDeathSound() {
    if (_graphics && isSfxEnabled())
        _graphics->PlaySound("assets/music/death.mp3");
}

void SoundManager::playBroadcastSound() {
    if (_graphics && isSfxEnabled())
        _graphics->PlaySound("assets/music/discord.mp3");
}

void SoundManager::playSound(const std::string& soundPath) {
    if (_graphics && isSfxEnabled())
        _graphics->PlaySound(soundPath);
}

bool SoundManager::isSfxEnabled() const {
    if (!_gameState)
        return false;
    bool enabled = _gameState->getSfxEnabled();
    return enabled;
}

void SoundManager::setGraphics(std::shared_ptr<IGraphicsLib> graphics) {
    _graphics = graphics;
}
